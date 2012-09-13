#
# Makefile for Arthur test
#

# FIXME -Wall -Weffc++ -Wextra

LIBRARY = arthur_test

# Compiler
CC = clang++
WARNINGS = -Wno-c++98-compat -Wno-weak-vtables
GTEST_FIX = -DGTEST_USE_OWN_TR1_TUPLE=1
CFLAGS = -g -Weverything -Wall -std=c++11 -stdlib=libc++ $(WARNINGS) $(GTEST_FIX) -I../ -I../../ -I/usr/local/include
LDFLAGS = -stdlib=libc++ -L/usr/local/boost_1_50_0/stage/lib -L/usr/local/lib -lboost_program_options -lboost_system -lglog -lgtest

# Object, headers and source definitions
OBJS =	$(patsubst %.cpp,%.o,$(SRCS)) 

HDRS = 	abstract_factory.hpp
SRCS = 	test/test_main.cpp \
		test/test_abstract_factory.cpp

# Dependencies

# Build
all: $(SRCS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.cpp.o: $(HDRS)
	$(CC) $(CFLAGS) $< -c -o $@

# Clean
clean:
	rm -f $(LIBRARY)
	rm -f $(OBJS)
