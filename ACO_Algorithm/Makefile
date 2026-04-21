#Makefile for CSC 2400 Group Project
#Builds dataGeneration and aco
#used https://www.geeksforgeeks.org/cpp/makefile-in-c-and-its-applications/
#and a bit of AI to help me understand how to make this Makefile
# Usage:
#   make            — build both programs
#   make run        — generate the graph, then run ACO on it
#   make clean      — remove built binaries and data file
 
CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
 
# Detect OS
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM  = del /Q /F
else
    EXE =
    RM  = rm -f
endif
 
.PHONY: all run clean
 
all: dataGeneration$(EXE) aco$(EXE)
 
# Compile the data generator (unchanged from original)
dataGeneration$(EXE): dataGeneration.cpp
	$(CXX) $(CXXFLAGS) -o dataGeneration$(EXE) dataGeneration.cpp
 
# Compile the ACO solver (header-only modules, so only main.cpp is a .cpp file)
aco$(EXE): main.cpp graph.h aco.h
	$(CXX) $(CXXFLAGS) -o aco$(EXE) main.cpp
 
# Generate the graph then immediately run ACO against it
run: dataGeneration$(EXE) aco$(EXE)
	.\dataGeneration$(EXE)
	.\aco$(EXE) thePoints.dat
 
clean:
	$(RM) dataGeneration$(EXE) aco$(EXE) thePoints.dat
