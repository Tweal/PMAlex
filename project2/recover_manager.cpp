#include "recovery_manager.hpp"

void RecoveryManager::recover(void) {
    // If recovery is not needed bail
    // we'll just start with an empty tree
    if (!isRecoveryNeeded())
        return;

    std::cout << "Starting Recovery" << std::endl;

    std::vector<std::string> lines = injestLines();

    for (std::string line : lines) {
        if (line.find("CHECKPOINT") != std::string::npos) 
            cpm->restoreFromCheckpoint(line);
        else 
            lm->executeLogLine(line);
    }

    std::cout << "Recovery Success" << std::endl;
}

bool RecoveryManager::isRecoveryNeeded(void) {
    std::ifstream file(logFilePath);

    // If we can open the file we should attempt recovery
    bool shouldRecover = file.is_open();

    // but only if the file is not empty
    file.seekg(0, std::ios::end);
    shouldRecover &= (file.tellg() != 0);

    file.close();

    return shouldRecover;
}

std::vector<std::string> RecoveryManager::injestLines(void) {
    std::vector<std::string> lines;
    std::ifstream file(logFilePath);

    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}