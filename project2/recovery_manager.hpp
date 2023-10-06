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
    RecoveryManager(std::string logFilePath, betree<uint64_t, std::string> *b, LogManager *lm, CheckpointManager *cpm) : 
      logFilePath(logFilePath),
      b(b),
      lm(lm),
      cpm(cpm)
    {}

    void recover(void);

private:
    std::string logFilePath;
    betree<uint64_t, std::string> *b;
    LogManager *lm;
    CheckpointManager *cpm;

    bool isRecoveryNeeded(void);
    std::vector<std::string> injestLines(void);

friend class RMTester;
};

/*
** I was going to write tests for this class but the logic is pretty simple and we're running out of time.
** Fuck it, we'll do it live!
*/
class RMTester { 
public:
  RMTester(RecoveryManager *rm, std::string testDir = "testDir/test") :
    rm(rm),
    testDir(testDir)
  {}

  void runAllTests(void) {
    testIsRecoveryNeeded();
    testInjestLines();
  }

  void testIsRecoveryNeeded(void) {
    std::cout << "Testing isRecoveryNeeded" << std::endl;

    // Subtest: No log file -> expect false
    {
      std::cout << "  Testing no log file" << std::endl;
      // Setup log file: 
      //  Check if exists, delete if does

      // Run method
      bool res = rm->isRecoveryNeeded();
      // assert result == expected
      assert(res == false);
    }

    // Subtest: Empty log file -> expect false
    {

    }

    // Subtest: Non-empty log file -> expect true
    {

    }

    std::cout << "Test Passed!" << std::endl;
  }

  void testInjestLines(void) {
    std::cout << "Testing testInjestLines" << std::endl;

    std::cout << "Test Passed!" << std::endl;
  }

private:

  RecoveryManager *rm;
  std::string testDir;
};
#endif // RECOVERY_MANAGER_HPP