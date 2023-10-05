// log_manager.hpp
/*
#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <map>
#include <vector>
#include <cassert>
#include "swap_space.hpp"
#include "backing_store.hpp"
#include "betree.hpp"

#include <string>
#include <iostream>
#include <fstream>

//probably a mess, just have it in betree
class betree
{};

class logger
{
  //where should this be passed? 
  int loggranularity;
  int cpgranularity;
  int cpcounter;
  std::vector<std::string> loglist;

  public:
    void flushlog(betree & mybtree)
    {
      std::ofstream myfile;
      myfile.open("logfile.txt", std::ios_base::app); //its being called and created, but not saving to file correctly

      //std::cout << "log opened \n";

      for(int i = 0; i < loggranularity; i++)
      {
        //std::cout << "writing log \n";
        std::string oldrequest = loglist[i];
        //std::cout << oldrequest;
        myfile << oldrequest;
      }
      //std::cout << "closing log \n";

      myfile.close();

      //std::cout << "log closed \n";

      //now actually apply the changes to the tree -> 
      for(int i = 0; i < loggranularity; i++)
      {

        //std::cout << "applying changes to tree \n";
        std::string newrequest = loglist[i];
        //std::cout << "getting request from log \n";
        //std::cout << newrequest;


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

        //std::cout << "request parsed \n";
        //std::cout << "changing tree \n";

        //original code 

        //need a reference to b-tree -> pass self as reference ==>> go back to original strat 

        //go to betree -> it needs logger definitions -> that needs betree definitions -> bruh

        betree::message_map tmp;
        tmp[betree::MessageKey<betree::Key>(k, mybtree.next_timestamp++)] = betree::Message<betree::Value>(opcode, v);
        //pivot_map new_nodes = root->flush(*this, tmp);
        betree::pivot_map new_nodes = mybtree.root->flush(*mybtree, tmp);

        if (new_nodes.size() > 0)
        {
          mybtree.root = mybtree.ss->allocate(new node);//ss? is a pointer to a swapspace?
          mybtree.root->pivots = new_nodes;
        }

        //std::cout << "tree changed \n";

      }

      //empty the logger after the tree building is complete
      while(loglist.size() > 0)
      {
        loglist.pop_back();

      }

      //increment checkpoint counter
      cpcounter++;
    }

    void addelement(std::string log)
    {
      loglist.push_back(log);
    }
    void popback()
    {
      loglist.pop_back();
    }
    std::string getlog(int i)
    {
      return loglist[i];
    }
    int getlogsize()
    {
      return loglist.size();
    }
    int getloggranularity()
    {
      return loggranularity;
    }

    int getcpcounter()
    {
      return cpcounter;
    }
    void inccpcounter()
    {
      cpcounter++;
    }
    void resetcpcounter()
    {
      cpcounter = 0;
    }

    logger(int i, int j)
    {
      loggranularity = i;
      cpgranularity = j;
      cpcounter = 0;
    }
    logger()
    {
      loggranularity = 10;
      cpgranularity = 10;
      cpcounter = 0;
    }
};

#endif // LOG_MANAGER_HPP

*/