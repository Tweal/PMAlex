
ifdef D
   CXXFLAGS=-Wall -std=c++11 -g -pg -DDEBUG
else
   CXXFLAGS=-Wall -std=c++11 -g -O3 
endif



#CXXFLAGS=-Wall -std=c++11 -g -pg

CC=g++

all: test test_logging_restore generate test_checkpointing

test: test.cpp betree.hpp swap_space.o backing_store.o log_manager.hpp checkpoint_manager.o

test_logging_restore: test_logging_restore.cpp betree.hpp swap_space.o backing_store.o log_manager.o checkpoint_manager.o recovery_manager.o

unit_tests: unit_tests.cpp betree.hpp swap_space.o backing_store.o log_manager.o checkpoint_manager.o recovery_manager.o

generate: generate.cpp

swap_space.o: swap_space.cpp swap_space.hpp backing_store.hpp

backing_store.o: backing_store.hpp backing_store.cpp

checkpoint_manager.o: checkpoint_manager.hpp checkpoint_manager.cpp

log_manager.o: log_manager.hpp log_manager.cpp

recovery_manager.o: recovery_manager.hpp recovery_manager.cpp

clean:
	$(RM) *.o test test_logging_restore unit_tests generate
