CXX=g++
CXXFLAGS=-Wall -g
.SUFFIXES = .cpp

.PHONY:all
all: visa
visa: visa.cpp disk_sched.hpp
	$(CXX) $(CXXFLAGS) -std=c++11 -o $@ $< 
