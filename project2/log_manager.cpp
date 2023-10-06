#include "log_manager.hpp"
#include <map>
#include <vector>
#include <cassert>
#include "swap_space.hpp"
#include "backing_store.hpp"

#include <string>
#include <iostream>
#include <fstream>


void LogManager::flushlog()
  {
    // guard clause if nothing to flush
    if(logList.size() == 0)
      return;

    std::ofstream myfile;
    myfile.open(logFilePath, std::ios_base::app); 
    

    for(uint64_t i = 0; i < logGranularity; i++)
    {
      std::string oldrequest = logList[i];
      myfile << oldrequest << std::endl;
    }

    myfile.close();

    for(uint64_t i = 0; i < logGranularity; i++)
    {
      executeLogLine(logList[i]);
    }

    while(logList.size() > 0)
    {
      //empty backing array
      logList.pop_back();
    }

  }

void LogManager::executeLogLine(std::string line) {
    int opcode;
    uint64_t k;
    std::string v;
    std::tie(opcode, k, v) = parseLine(line);
    b->upsert(opcode, k, v);
}

std::tuple<int, uint64_t, std::string> LogManager::parseLine(std::string line) {
  std::istringstream iss(line);
  int opcode;
  uint64_t k;
  std::string v;
  
  if(!(iss >> opcode >> k >> v))
    throw std::runtime_error("Failed to parse " + line);
    
  return std::make_tuple(opcode, k, v);
}

void LogManager::addelement(std::string log)
    {
      logList.push_back(log);
    }
void LogManager::popback()
    {
      logList.pop_back();
    }
std::string LogManager::getlog(uint64_t  i)
    {
      return logList[i];
    }
uint64_t  LogManager::getlogsize()
    {
      return logList.size();
    }
uint64_t  LogManager::getloggranularity()
    {
      return logGranularity;
    }
    


