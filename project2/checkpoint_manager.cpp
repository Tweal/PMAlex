#include "checkpoint_manager.hpp"

bool CheckpointManager::createCheckpoint() {
    std::cout << "Checkpointing Started" << std::endl;
    // Flush Log
    lm.flushlog();

    // Push all objects to disk
    b.dump_to_disk();

    // Write Checkpoint start to log, include root info.
    std::ofstream logFile(logFilePath, std::ios::out | std::ios::trunc);
    auto rootInfo = b.get_root().get_pin().get_info();
    uint64_t rootID = std::get<0>(rootInfo);
    uint64_t rootVersion = std::get<1>(rootInfo);
    logFile << "CHECKPOINT rootID:" << rootID << " rootVersion:" << rootVersion << " ... ";

    // Delete old versions from disk. This could cause issues if we crash mid delete, hence the COMPLETE message is needed
    bool succeeded = deleteOldVersions();

    // Write Checkpoint Complete to log only if we succeeded in deleting everything
    if(succeeded)
        logFile << "COMPLETE" << std::endl;

    // Close log
    logFile.close();
    std::cout << "Checkpoint Finished! " << std::endl;
    return true;
}

bool CheckpointManager::deleteOldVersions(bool checkIfNeeded){

    bool success = true;
    
    std::vector<FileInfo> files = getFiles();

    std::map<uint64_t, uint64_t> newestVersions = getIdVersionMap(false);

    for (const FileInfo& file : files) {
        // We will need to delete files if the version is not the highest or if the id is no longer needed. The second case would be if that node was deleted
        if (file.version < newestVersions[file.id] ||
            (!b.id_needed(file.id) && checkIfNeeded)) {
            std::string filePath = bsDir + "/" + file.fileName;
            if(remove(filePath.c_str()) != 0){
                std::cerr << "Error deleting file: " << file.fileName << std::endl;
                success = false;
            }
            else{
                debug(std::cout << "Deleted: " << file.fileName << std::endl);
            }
        }
    }
    
    return success;
}


bool CheckpointManager::restoreFromCheckpoint(std::string checkpointStr) {
    auto parsedStr = parseCheckpointString(checkpointStr);
    uint64_t rootID = std::get<0>(parsedStr);
    uint64_t rootVersion = std::get<1>(parsedStr);
    bool complete = std::get<2>(parsedStr);

    std::cout << "Restoring from root " << std::to_string(rootID) << "_" << std::to_string(rootVersion) << std::endl;


    // If the complete boolean is false we know that we crashed
    // before finishing deleting files, so we need to finish
    // deleting before restoring
    if (!complete) {
        deleteOldVersions(false);
        // Then we can append COMPLETE to log
        std::ofstream logFile(logFilePath, std::ios_base::app);
        logFile << "COMPLETE" << std::endl;
        logFile.close();
    }

    // Now we need to get the map of file ids -> versions
    std::map<uint64_t, uint64_t> idVersionMap = getIdVersionMap(true);

    // And now we can restore from this root with this info
    b.restore(std::make_pair(rootID, rootVersion), idVersionMap);

    return true;
}

std::vector<FileInfo> CheckpointManager::getFiles() {
    std::vector<FileInfo> files;
    DIR* dir = opendir(bsDir.c_str());

    if (dir) {
        struct dirent* entry;

        while ((entry = readdir(dir))) {
            std::string fileName = entry->d_name;

            size_t underscorePos = fileName.find('_');
            size_t dotPos = fileName.find('.');
            
            if (underscorePos != std::string::npos && dotPos != std::string::npos) {
                std::string idStr = fileName.substr(0, underscorePos);
                std::string versionStr = fileName.substr(underscorePos + 1, dotPos - underscorePos - 1);

                try {
                    uint64_t id = std::stoull(idStr);
                    uint64_t version = std::stoull(versionStr);

                    files.push_back({fileName, id, version});
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("Failed to parse ints: " + idStr + ", " + versionStr);
                }
            }
        }
        closedir(dir);
    } else
        throw std::runtime_error("Failed to open bs directory!");

    return files;
}

std::map<uint64_t, uint64_t> CheckpointManager::getIdVersionMap(bool getOldest) {
    std::vector<FileInfo> files = getFiles();
    
    std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
        if (a.id != b.id) {
            return a.id < b.id;
        } else {
            return a.version < b.version;
        }
    });

    std::map<uint64_t, uint64_t> idVersionMap;
    // Find the correct version for each id
    for (const FileInfo& file: files) {
        if (getOldest ?
                (file.version < idVersionMap[file.id]) :
                (file.version > idVersionMap[file.id])){
            idVersionMap[file.id] = file.version;
        }
    }

    return idVersionMap;
}

std::tuple<uint64_t, uint64_t, bool> CheckpointManager::parseCheckpointString(std::string checkpointStr){
    uint64_t rootID = -1;
    uint64_t rootVersion = -1;
    bool complete = false;

    size_t idPos = checkpointStr.find("rootID:");
    size_t versionPos = checkpointStr.find("rootVersion:");

    if (idPos != std::string::npos && versionPos != std::string::npos) {
        std::string idSubstring = checkpointStr.substr(idPos + 7);
        std::string versionSubstring = checkpointStr.substr(versionPos + 12);

        std::istringstream idStream(idSubstring);
        std::istringstream versionStream(versionSubstring);

        if (!(idStream >> rootID) || !(versionStream >> rootVersion)) {
            std::cerr << "Error: Failed to parse integers." << std::endl;
        }

        complete = (checkpointStr.find("COMPLETE") != std::string::npos);
    } else {
        std::cerr << "Error: Input string format is incorrect." << std::endl;
    }

    return std::make_tuple(rootID, rootVersion, complete);
}
