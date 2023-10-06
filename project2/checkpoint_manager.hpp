#ifndef CHECKPOINT_MANAGER_HPP
#define CHECKPOINT_MANAGER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>
#include <tuple>
#include <vector>
#include <limits>
#include <map>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>

#include "betree.hpp"
#include "log_manager.hpp"
#include "debug.hpp"

// Structure to store file information
struct FileInfo {
    std::string fileName;
    uint64_t id;
    uint64_t version;
};

class CheckpointManager {
public:
    CheckpointManager(std::string logFilePath, std::string bsDir, betree<uint64_t, std::string> *b, LogManager *logManager, uint64_t granularity = 10) :
        logFilePath(logFilePath),
        bsDir(bsDir),
        b(b),
        lm(logManager),
        granularity(granularity)
    {}

    CheckpointManager& operator++();
    CheckpointManager& operator++(int);

    // Method to create a checkpoint of the current system state.
    bool createCheckpoint(void);

    // Method to restore the system state from a checkpoint.
    bool restoreFromCheckpoint(std::string checkpointStr);

private:
    bool deleteOldVersions(bool checkIfNeeded = true);
    std::vector<FileInfo> getFiles(void);
    std::map<uint64_t, uint64_t> getIdVersionMap(bool getOldest);
    std::tuple<uint64_t, uint64_t, bool> parseCheckpointString(std::string checkpointStr);
    void increment(void);

    std::string logFilePath;
    std::string bsDir;
    betree<uint64_t, std::string> *b;
    LogManager *lm;
    uint64_t granularity;
    uint64_t counter = 0;
    
friend class CPTester;
};

class CPTester {
public:
    CPTester(CheckpointManager *cpm) :
        cpm(cpm)
    {} 

    void runAllTests(void) {
        testDeleteOldVersions();
        testParseCheckpointString();
    }

    void testDeleteOldVersions(void) {
        std::cout << "Testing deleteOldVersions" << std::endl;

        // Need to generate some fake files with names of the format "#_#.txt". Generates a version for each id, ie id 5 will have 5 versions
        createDummyFiles();

        // Now hopefully those files get deleted!
        cpm->deleteOldVersions(false);
        
        // Verify that there's only one file for each id and they all match the id as that's the highest version
        assert(verifyDir());

        // Clean up the directory
        DIR* dir = opendir(cpm->bsDir.c_str());

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            std::string filename = entry->d_name;
            if (filename.find("_") != std::string::npos) {
                remove((cpm->bsDir + "/" + filename).c_str());
            }                
        }
        closedir(dir);

        std::cout << "Test Passed!" << std::endl;
    }

    void testParseCheckpointString(void) {
        std::cout << "Testing parseCheckpointString" << std::endl;

        uint64_t id = 3;
        uint64_t version = 2;
        std::string cpStr = "CHECKPOINT rootID:" + std::to_string(id) + " rootVersion:" + std::to_string(version);
        std::cout << "  Testing with input: <" << cpStr << ">" << std::endl;
        auto parsedStr = cpm->parseCheckpointString(cpStr);
        assert(std::get<0>(parsedStr) == id);
        assert(std::get<1>(parsedStr) == version);
        assert(std::get<2>(parsedStr) == false);
        std::cout << "  Case passed!" << std::endl;

        cpStr += " ... COMPLETE";
        std::cout << "  Testing with input: <" << cpStr << ">" <<  std::endl;
        parsedStr = cpm->parseCheckpointString(cpStr);
        assert(std::get<2>(parsedStr) == true);
        std::cout << "  Case passed!" << std::endl;

        std::cout << "Test Passed!" << std::endl << std::endl;
    }


private:
    void createDummyFiles(void) {

        for (int id = 1; id <= 5; ++id) {
            int numVersions = id;

            for (int version = 1; version <= numVersions; version++) {
                std::ostringstream filename;
                filename << cpm->bsDir << "/" << id << "_" << version << ".txt";

                std::ofstream file(filename.str());

                if (file.is_open()) {
                    file.close();
                } else {
                    std::cerr << "Failed to create: " << filename.str() << std::endl;
                }
            }
        }
    }

    bool verifyDir(void) {
        // Get actual file names
        std::vector<std::string> actualFilenames;
        DIR* dir = opendir(cpm->bsDir.c_str());

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            std::string filename = entry->d_name;
            if (filename.find("_") != std::string::npos)
                actualFilenames.push_back(filename);
        }
        closedir(dir);

        // Generate expected file names
        std::vector<std::string> expectedFilenames;
        // These were deterministically generated to be "i_i.txt"
        for (int i = 1; i <= 5; i++){
            std::string filename = std::to_string(i) + "_" + std::to_string(i) + ".txt";
            if (filename.find("_") != std::string::npos)
            expectedFilenames.push_back(filename);
        }

        std::sort(actualFilenames.begin(), actualFilenames.end());
        std::sort(expectedFilenames.begin(), expectedFilenames.end());

        return actualFilenames == expectedFilenames;
    }

    CheckpointManager *cpm;
};

#endif // CHECKPOINT_MANAGER_HPP
