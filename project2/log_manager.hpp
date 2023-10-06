// log_manager.hpp

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

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

class LogManager {
public:
    LogManager(std::string logFilePath, betree<uint64_t, std::string> *b, uint64_t logGranularity = 10) :
      logFilePath(logFilePath),
      logGranularity(logGranularity),
      b(b)  
    {}

    void writeToLog(std::string transaction);
    void flushlog(void);
    void addelement(std::string log);
    void popback(void);
    void executeLogLine(std::string line); //TODO: THIS!

    std::string getlog(uint64_t  i);
    uint64_t  getlogsize(void);
    uint64_t  getloggranularity(void);

private:
    std::string logFilePath;
    uint64_t logGranularity;
    betree<uint64_t, std::string> *b;
    std::vector<std::string> logList;
};

#endif // LOG_MANAGER_HPP