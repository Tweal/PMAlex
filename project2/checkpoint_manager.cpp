#include "checkpoint_manager.hpp"
#include "log_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <limits>
#include <map>


bool CheckpointManager::createCheckpoint() {
    std::cout << "Checkpointing Started" << std::endl;
    // Flush Log
    lm.flushlog();

    // Push all objects to disk
    b.dump_to_disk();

    // Write Checkpoint start to log, include root info.
    std::ofstream logFile(logFilePath, std::ios_base::app);
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

bool CheckpointManager::deleteOldVersions(bool testing){

    DIR* dir = opendir(bsDir.c_str());

    bool success = true;

    if (dir) {
        struct dirent* entry;

        std::vector<FileInfo> files;

        while ((entry = readdir(dir))) {
            std::string fileName = entry->d_name;

            size_t underscorePos = fileName.find('_');
            size_t dotPos = fileName.find('.');
            
            if (underscorePos != std::string::npos && dotPos != std::string::npos) {
                std::string idStr = fileName.substr(0, underscorePos);
                std::string versionStr = fileName.substr(underscorePos + 1, dotPos - underscorePos - 1);

                try {
                    int id = std::stoi(idStr);
                    int version = std::stoi(versionStr);

                    files.push_back({fileName, id, version});
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Failed to parse ints: " << idStr << ", " << versionStr << std::endl;
                    success = false;
                }
            }
        }
        closedir(dir);

        std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
            if (a.id != b.id) {
                return a.id < b.id;
            } else {
                return a.version < b.version;
            }
        });

        std::map<int, int> newestVersions;
        // Find the highest version for each id
        for (const FileInfo& file: files) {
            if (file.version > newestVersions[file.id]){
                newestVersions[file.id] = file.version;
            }
        }

        for (const FileInfo& file : files) {
            // We will need to delete files if the version is not the highest or if the id is no longer needed. The second case would be if that node was deleted
            if (file.version < newestVersions[file.id] || (!b.id_needed(file.id) && !testing)) {
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
    } else {
        std::cerr << "Failed to open directory." << std::endl;
        success = false;
    }

    return success;
}


bool CheckpointManager::restoreFromCheckpoint(std::string checkpointStr) {
    auto parsedStr = parseCheckpointString(checkpointStr);
    uint64_t rootID = std::get<0>(parsedStr);
    uint64_t rootVersion = std::get<1>(parsedStr);
    bool complete = std::get<2>(parsedStr);

    // If the complete boolean is false we know that we crashed before finishing deleting files, so we need to restore from newest versions (so that ss->objects has all the ids) and then finish deleting
    if (!complete) {

    }
    // Else we can just restore from oldest
    else {

    }

    return true;
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
