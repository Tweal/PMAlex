#ifndef RECOVERY_MANAGER_HPP
#define RECOVERY_MANAGER_HPP


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
#include "checkpoint_manager.hpp"
#include "debug.hpp"

class RecoveryManager {
public: 
    RecoveryManager(std::string logFilePath, betree<uint64_t, std::string>* b, LogManager* lm, CheckpointManager* cpm) : 
      logFilePath(logFilePath),
      b(b),
      lm(lm),
      cpm(cpm)
    {}

    void recover(void);

private:
    std::string logFilePath;
    betree<uint64_t, std::string>* b;
    LogManager* lm;
    CheckpointManager* cpm;

    bool isRecoveryNeeded(void);
    std::vector<std::string> injestLines(void);
};

#endif // RECOVERY_MANAGER_HPP