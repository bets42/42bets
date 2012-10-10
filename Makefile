#
# Non-recursive makefile for 42bets project
#
# Google "Recursive Make Considered Harmful pdf" for the seminal paper 
# outlining the rationale for using non-recursive make and the design 
# of the (small) build system implemented for this project.
#

# TODO:
# 1. Configurable debug/optimised build
# 2. Configurable gcc/clang build
# 3. split out -DGTEST_USE_OWN_TR1_TUPLE=1 from CFLAGS to TEST_CFLAGS
# 4. Ensure header-only changes result in rebuild of all dependents


# --------- GCC CONFIG ---------

GCC := /opt/local/bin/g++-mp-4.7

GCC_WARNINGS := 

GCC_CFLAGS := \
	-g -Wall -Weffc++ -Wextra -std=c++11

GCC_LDFLAGS :=



# --------- CLANG CONFIG ---------

CLANG := /opt/local/bin/clang++-mp-3.1

CLANG_WARNINGS :=					\
	-Wno-c++98-compat 				\
	-Wno-exit-time-destructors		\
	-Wno-global-constructors 		\
	-Wno-padded 					\
	-Wno-weak-vtables  	

CLANG_CFLAGS := \
	-g -Weverything -Wall -std=c++11 -stdlib=libc++
 
CLANG_LDFLAGS := \
	-stdlib=libc++



#--------- GENERIC COMPILER CONFIG ---------

CC :=		$(CLANG)
CFLAGS :=	$(CLANG_WARNINGS) $(CLANG_CFLAGS) -DGTEST_USE_OWN_TR1_TUPLE=1
LDFLAGS := 	$(CLANG_LDFLAGS)

INCS :=     						\
	-I. 	                        \
	-I/usr/local/include            \
	$(patsubst %, -I%, $(SUBDIRS))

LIBS := 					\
	-L/usr/local/ssl/lib	\
	-lssl					\
	-lcrypto				\
	-L/usr/local/lib		\
	-lboost_program_options	\
	-lboost_system 			\
	-lcurl					\
	-lglog 					\
	-lpugixml

TEST_LIBS := 	\
	$(LIBS)		\
   	-lgtest 	\
	-lgtest_main



# --------- GENERAL BUILD VARS ---------

BUILD_DIR := build



# --------- BINARY VARS ---------
# module.mk files in each of the sub-directories append to
# SRCS to build up a complete list of source files

SRCS :=
SUBDIRS := 						\
	bets42/arthur				\
	bets42/deepthgt				\
	bets42/deepthgt/betfair		\
	bets42/example/tick_capture	\
	bets42/marvin
 
include $(patsubst %, %/module.mk, $(SUBDIRS))

OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(SRCS)))

-include $(OBJS:.o=.d)



# --------- TEST VARS ---------
# module.mk files in each of the sub-directories append to
# TEST_SRCS to build up a complete list of source files

TEST_SRCS :=
TEST_SUBDIRS := \
	test/bets42/arthur

include $(patsubst %, %/module.mk, $(TEST_SUBDIRS))

TEST_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(SRCS) $(TEST_SRCS)))

-include $(TEST_OBJS:.o=.d)
                            


# --------- make TARGETS ---------
# manually include bets42/Main.cpp instead of adding to SRCS 
# so that TEST_SRCS doesn't pick this up as its main() function, 
# it uses the main() function provided by the gtest_main library

BINARY 		:= 42bets
TEST_BINARY := $(BINARY)-test

.PHONY: all binary test clean

all: binary test

binary: $(BINARY)

test: $(TEST_BINARY)

$(BINARY): $(BUILD_DIR)/bets42/Main.o $(OBJS)
	$(CC) -o $@ $(BUILD_DIR)/bets42/Main.o $(OBJS) $(LDFLAGS) $(LIBS)

$(TEST_BINARY): $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(LDFLAGS) $(TEST_LIBS)

vpath %.cpp .

$(BUILD_DIR)/%.d: %.cpp
	mkdir -p `dirname $@`
	./scripts/depend.sh `dirname $*.cpp` $(INCS) $*.cpp > $@

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCS) $< -c -o $@

clean:
	rm -f $(BINARY)
	rm -f $(TEST_BINARY)
	rm -rf $(BUILD_DIR)

# --------- et voila! ---------
