#
# Makefile for 42Bets
#

# -Wall -Weffc++ -Wextra

CC = clang++
WARNINGS = -Wno-c++98-compat -Wno-weak-vtables
CFLAGS = -g -Weverything -Wall -std=c++11 -stdlib=libc++ $(WARNINGS) -I../ -I/usr/local/include 
LDFLAGS = -stdlib=libc++ -L/usr/local/boost_1_50_0/stage/lib -L/usr/local/lib -lboost_program_options -lboost_system -lglog

LIBRARY = 42bets

OBJS =	$(patsubst %.cpp,%.o,$(SRCS)) 

HDRS = 	arthur/entry_exit.hpp \
		arthur/exception.hpp \
		arthur/memory.hpp \
		deepthgt/CommandHandler.hpp \
		deepthgt/Exception.hpp \
		deepthgt/TradingEngine.hpp \
		deepthgt/TradingEngineImpl.hpp
SRCS = 	Main.cpp \
		deepthgt/CommandHandler.cpp \
		deepthgt/TickCaptureAlgo.cpp

all: $(SRCS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.cpp.o: $(HDRS)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(LIBRARY)
	rm -f $(OBJS)
