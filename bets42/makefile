#
# Makefile for 42Bets
#

# -Wall -Weffc++ -Wextra

CC = /opt/local/bin/clang++-mp-3.1
#CC = /opt/local/bin/g++-mp-4.7
WARNINGS = -Wno-c++98-compat -Wno-weak-vtables -Wno-padded -Wno-global-constructors -Wno-exit-time-destructors
CFLAGS = -g -Weverything -Wall -std=c++11 -stdlib=libc++ $(WARNINGS) -I../ -I/usr/local/include 
#CFLAGS = -g -Weffc++ -Wextra -Wall -std=c++11 $(WARNINGS) -I../ -I/usr/local/include 
LDFLAGS = -stdlib=libc++ -L/usr/local/boost_1_50_0/stage/lib -L/usr/local/lib -lboost_program_options -lboost_system -lglog

LIBRARY = 42bets

OBJS =	$(patsubst %.cpp,%.o,$(SRCS)) 

SRCS = 	Main.cpp \
		deepthgt/Algo.cpp \
		deepthgt/CommandHandler.cpp \
		deepthgt/Exchange.cpp \
		deepthgt/TCPSocket.cpp \
		deepthgt/TradingEngine.cpp \
		deepthgt/betfair/Exchange.cpp \
		earth/tick_capture/TickCapture.cpp \
		marvin/types.cpp

all: $(SRCS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(LIBRARY)
	rm -f $(OBJS)
