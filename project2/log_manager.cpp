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
      std::ofstream myfile;
      myfile.open("logfile.txt", std::ios_base::app); 

      for(uint64_t i = 0; i < logGranularity; i++)
      {
        std::string oldrequest = logList[i];
        myfile << oldrequest;
      }

      myfile.close();

      for(uint64_t i = 0; i < logGranularity; i++)
      {

        std::string newrequest = logList[i];
        std::string token;
        std::vector<std::string> index;

        for(size_t i = 0; i < newrequest.length(); i++)
        {
          char c = newrequest[i];
          if(!(c == ':'))
          {
            token += newrequest[i];
          }
          else
          {
            index.push_back(token);
            token = "";            
          }
        }

        int opcode = stoi(index[0]);
        uint16_t k = stoull(index[1]); 
        std::string v = index[2] + ":";

        //could be much better encapsulated but this is decently efficient
        b->upsert(opcode, k, v); 
      }

      while(logList.size() > 0)
      {
        //empty backing array
        logList.pop_back();

      }

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
    


