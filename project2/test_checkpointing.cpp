#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "betree.hpp"
#include "log_manager.hpp"
#include "checkpoint_manager.hpp"

#define DEFAULT_TEST_MAX_NODE_SIZE (1ULL << 6)
#define DEFAULT_TEST_MIN_FLUSH_SIZE (DEFAULT_TEST_MAX_NODE_SIZE / 4)
#define DEFAULT_TEST_CACHE_SIZE (4)



int main(int argc, char **argv){
    uint64_t max_node_size = DEFAULT_TEST_MAX_NODE_SIZE;
    uint64_t min_flush_size = DEFAULT_TEST_MIN_FLUSH_SIZE;
    uint64_t cache_size = DEFAULT_TEST_CACHE_SIZE;
    std::string testDir = "testdir";
    std::string logFile = testDir + "/logfile.txt";

    one_file_per_object_backing_store ofpobs(testDir);
    swap_space sspace(&ofpobs, cache_size);
    betree<uint64_t, std::string> b(&sspace, max_node_size, min_flush_size);

    LogManager lm(logFile);

    CheckpointManager cpm(logFile, testDir, b, lm);

    CPTester tester(cpm);
    tester.testParseCheckpointString();
    tester.testDeleteOldVersions();
}