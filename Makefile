#
# Non-recursive makefile for 42bets project
#
# Google "Recursive Make Considered Harmful pdf" for the seminal paper 
# outlining the rationale for using non-recursive make and the design 
# of the (small) build system implemented for this project.
#
# https://github.com/42bets
#

# TODO:
# 1. Configurable debug/optimised build
# 2. Configurable gcc/clang build
# 3. split out -DGTEST_USE_OWN_TR1_TUPLE=1 from CFLAGS to TEST_CFLAGS
# 4. write *.o and *.d to seperate directories away from source
# 5. Ensure header-only changes result in rebuild of all dependents


# --------- GCC CONFIG ---------

GCC := /opt/local/bin/g++-mp-4.7

GCC_WARNINGS := 

GCC_CFLAGS := \
	-g -Wall -Weffc++ -Wextra -std=c++11

GCC_LDFLAGS :=



# --------- CLANG CONFIG ---------

CLANG := /opt/local/bin/clang++-mp-3.1

CLANG_WARNINGS :=				\
	-Wno-c++98-compat 			\
	-Wno-weak-vtables  	 		\
	-Wno-padded 				\
	-Wno-global-constructors 	\
	-Wno-exit-time-destructors

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

LIBS := 								\
	-L/usr/local/boost_1_50_0/stage/lib \
	-L/usr/local/lib					\
	-lboost_program_options 			\
	-lboost_system 						\
	-lglog 								\
	-lpugixml

TEST_LIBS := 	\
	$(LIBS)		\
   	-lgtest 	\
	-lgtest_main



# --------- BINARY VARS ---------

SRCS :=
SUBDIRS := 						\
	bets42/arthur				\
	bets42/deepthgt				\
	bets42/deepthgt/betfair		\
	bets42/earth/tick_capture	\
	bets42/marvin
 
include $(patsubst %, %/module.mk, $(SUBDIRS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))

DEPS := $(OBJS:.o=.d)
-include $(DEPS)    



# --------- TEST VARS ---------

TEST_SRCS :=
TEST_SUBDIRS := \
	test/bets42/arthur

include $(patsubst %, %/module.mk, $(TEST_SUBDIRS))

TEST_OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS) $(TEST_SRCS)))

TEST_DEPS := $(TEST_OBJS:.o=.d)
-include $(TEST_DEPS)
                            


# --------- make TARGETS ---------

BINARY 		:= 42bets
TEST_BINARY := $(BINARY)-test

all: binary test

binary: $(BINARY)

test: $(TEST_BINARY)

$(BINARY): bets42/Main.o $(OBJS)
	$(CC) -o $@ bets42/Main.o $(OBJS) $(LDFLAGS) $(LIBS)

$(TEST_BINARY): $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(LDFLAGS) $(TEST_LIBS)

%.d: %.cpp
	./scripts/depend.sh `dirname $*.cpp` $(INCS) $*.cpp > $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCS) $< -c -o $@

clean:
	rm -f $(BINARY)
	rm -f $(TEST_BINARY)
	rm -f $(OBJS) bets42/Main.o bets42/Main.d
	rm -f $(TEST_OBJS)
	rm -f $(DEPS)
	rm -f $(TEST_DEPS)

# --------- et voila! ---------
