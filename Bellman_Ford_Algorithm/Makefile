#	Filename:		Makefile											  	#
#	Date: 			04/20/26												#
#	Authors: 		Erick Ramirez (Code Based on Eli DeSha Makefile)		#
#	Purpose: 		Creates and generates all data and results				#
#	Instructions:	Run the commade 'make full' with dataGeneration.cpp		#
#					and Main.cpp in the same directory. The Makefile		#
#					will create both executables, run dataGeneration.cpp	#
#					first to generate the data, then use that data to		#
#					generate results.txt, and finally clean both			#
#					executables from the folder.							#
CXX = g++
# Compiler flags:
# -std=c++17 = use C++17 standard
# -O2 = optimization for faster execution
CXXFLAGS = -std=c++17 -O2
# Names of output executable files
GEN_EXE = dataGen         # data generation program
MAIN_EXE = bellmanFord    # Bellman-Ford algorithm program
# Source files
GEN_SRC = dataGeneration.cpp
MAIN_SRC = bellmanFord.cpp
# Default target: runs "make" → executes "run"
all: run
# Compile the data generation program
$(GEN_EXE): $(GEN_SRC)
	$(CXX) $(CXXFLAGS) -o $(GEN_EXE) $(GEN_SRC)
# Compile the Bellman-Ford program
$(MAIN_EXE): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXE) $(MAIN_SRC)
# Run both programs in order:
# 1. Generate data file (thePoints.dat)
# 2. Run Bellman-Ford algorithm on generated data
run: $(GEN_EXE) $(MAIN_EXE)
	./$(GEN_EXE)
	./$(MAIN_EXE)
# Clean step (Linux/Mac safe version)
# Deletes compiled executables and generated data/result files
clean:
	rm -f $(GEN_EXE) $(MAIN_EXE) thePoints.dat bellmanFord_result.txt
# Full pipeline:
# 1. Build both programs
# 2. Run data generation
# 3. Run Bellman-Ford program
# 4. Delete executables afterward (keeps thePoints.dat and bellmanFord_result.txt)
full: $(GEN_EXE) $(MAIN_EXE)
	./$(GEN_EXE)
	./$(MAIN_EXE)
	rm -f $(GEN_EXE) $(MAIN_EXE)
