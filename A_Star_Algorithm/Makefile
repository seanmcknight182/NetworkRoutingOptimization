#---------------------------------------------------------------------------#
#	Filename:		Makefile											  	#
#	Date: 			04/20/26												#
#	Authors: 		ChatGPT and Eli DeSha									#
#	Purpose: 		Creates and generates all data and results				#
#	Instructions:	Run the commade 'make full' with dataGeneration.cpp		#
#					and Main.cpp in the same directory. The Makefile		#
#					will create both executables, run dataGeneration.cpp	#
#					first to generate the data, then use that data to		#
#					generate results.txt, and finally clean both			#
#					executables from the folder.							#
#---------------------------------------------------------------------------#

# Compiler to use (G++)
CXX = g++

# Compiler flags:
# -std=c++17 = use C++17 standard
# -O2 = optimization for faster execution
CXXFLAGS = -std=c++17 -O2

# Names of output executable files
GEN_EXE = dataGen     # data generation program
MAIN_EXE = main       # A* algorithm program

# Source files
GEN_SRC = dataGeneration.cpp
MAIN_SRC = Main.cpp

# Default target: runs "make" → executes "run"
all: run

# Compile the data generation program
$(GEN_EXE): $(GEN_SRC)
	$(CXX) $(CXXFLAGS) -o $(GEN_EXE) $(GEN_SRC)

# Compile the main A* program
$(MAIN_EXE): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXE) $(MAIN_SRC)

# Run both programs in order:
# 1. Generate data file
# 2. Run A* algorithm on generated data
run: $(GEN_EXE) $(MAIN_EXE)
	./$(GEN_EXE)
	./$(MAIN_EXE)

# Clean step (Windows safe version)
# Deletes compiled .exe files
clean:
	@if exist dataGen.exe del /Q dataGen.exe
	@if exist main.exe del /Q main.exe

# Full pipeline:
# 1. Build both programs
# 2. Run data generation
# 3. Run A* program
# 4. Delete executables afterward
full: $(GEN_EXE) $(MAIN_EXE)
	./$(GEN_EXE)
	./$(MAIN_EXE)
	if exist dataGen.exe del /Q dataGen.exe
	if exist main.exe del /Q main.exe
