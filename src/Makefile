################################################################################
# Makefile for HMM tagger
################################################################################

################################################################################
# 1) Choose C++ compiler.
################################################################################

CXX = g++

################################################################################
# 2) Set C++ flags.
#    a) DEBUG mode -- no optimizations, enable SafeVector checking, no inlining
#    b) PROFILE mode -- for gprof
#    c) RELEASE mode
################################################################################


CXXFLAGS = -O3 -W -Wall
# debug mode
#CXXFLAGS = -g

################################################################################
# 3) Dependencies
################################################################################

TARGETS = hmmtagger
DEPS =  tokenizer.h hmmtagger.h 
OBJS =  tokenizer.o hmmtagger.o main.o

all : $(TARGETS)
hmmtagger :	$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	
tokenizer.o : tokenizer.cpp 
	$(CXX) -c  -o $@ $< $(CXXFLAGS)
	
hmmtagger.o : hmmtagger.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	
main.o :main.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS)


.PHONY : clean
clean:
	rm -f $(TARGETS) *.o *.h~ *.hpp~ *.cpp~ *.cc~ 
