CXX = g++
CXXFLAGS = -std=c++17 -O2

# Algorithm directories
ALGO_DIRS = ACO_Algorithm A_Star_Algorithm Algorithm1Dijkstra Bellman_Ford_Algorithm

# Executables
DATA_GEN = dataGen
ALGO_EXE = runAlgo

all: run

# Compile everything
compile: $(DATA_GEN) algos

$(DATA_GEN): dataGeneration.cpp
	$(CXX) $(CXXFLAGS) dataGeneration.cpp -o $(DATA_GEN)

algos:
	for dir in $(ALGO_DIRS); do \
		$(CXX) $(CXXFLAGS) $$dir/main.cpp -o $$dir/$(ALGO_EXE); \
	done

# Run full pipeline
run: compile generate distribute execute python clean

# Generate dataset
generate:
	./$(DATA_GEN)

# Copy dataset into each algorithm folder
distribute:
	for dir in $(ALGO_DIRS); do \
		cp thePoints.dat $$dir/; \
	done

# Run each algorithm
execute:
	for dir in $(ALGO_DIRS); do \
		cd $$dir && ./$(ALGO_EXE); \
		cd ..; \
	done

# Run Python script
python:
	python3 script.py

# Cleanup executables (keeps results.txt and .dat files)
clean:
	rm -f $(DATA_GEN)
	for dir in $(ALGO_DIRS); do \
		rm -f $$dir/$(ALGO_EXE); \
	done
