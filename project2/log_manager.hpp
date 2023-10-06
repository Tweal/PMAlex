// log_manager.hpp

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <string>

class LogManager {
public:
    LogManager(const std::string logFilePath, uint64_t logGranularity, betree<uint64_t, std::string> b) :
      logFilePath(logFilePath),
      logGranularity(logGranularity),
      b(b)  
    {}

    void writeToLog(const std::string transaction);
    void flushlog(void);
    void addelement(std::string log);
    void popback(void);
    std::string getlog(uint64_t  i);
    uint64_t  getlogsize(void);
    uint64_t  getloggranularity(void);

private:
    std::string logFilePath;
    betree<uint64_t, std::string> b;
    uint64_t logGranularity;
    std::vector<std::string> logList;
};

#endif // LOG_MANAGER_HPP