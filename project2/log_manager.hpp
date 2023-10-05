// log_manager.hpp

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <string>

class LogManager {
public:
    LogManager(const std::string logFilePath) :
      logFilePath(logFilePath)  
    {}

    void writeToLog(const std::string transaction);
    void recoverFromLog(void);
    void flushlog(void){};

private:
    std::string logFilePath;
};

#endif // LOG_MANAGER_HPP