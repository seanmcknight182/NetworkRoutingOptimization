CXX = g++
CXXFLAGS = -std=c++17 -O2

DATA_GEN = dataGen
ALGO_EXE = runAlgo

SIZES = 150 300 600 1200 2400 4800 9600 19200

ACO = ACO_Algorithm
ASTAR = A_Star_Algorithm
DIJKSTRA = Algorithm1Dijkstra
BELLMAN = Bellman_Ford_Algorithm

all: run

# ========================
# BUILD
# ========================
compile: $(DATA_GEN) algos

$(DATA_GEN): dataGeneration.cpp
	$(CXX) $(CXXFLAGS) dataGeneration.cpp -o $(DATA_GEN)

algos:
	$(CXX) $(CXXFLAGS) $(ACO)/main.cpp -o $(ACO)/$(ALGO_EXE)
	$(CXX) $(CXXFLAGS) $(ASTAR)/main.cpp -o $(ASTAR)/$(ALGO_EXE)
	$(CXX) $(CXXFLAGS) $(DIJKSTRA)/main.cpp -o $(DIJKSTRA)/$(ALGO_EXE)
	$(CXX) $(CXXFLAGS) $(BELLMAN)/main.cpp -o $(BELLMAN)/$(ALGO_EXE)

# ========================
# RUN PIPELINE
# ========================
run: compile loop clean

loop:
	$(MAKE) run300
	$(MAKE) run600
	$(MAKE) run1200
	$(MAKE) run2400
	$(MAKE) run4800
	$(MAKE) run9600
	$(MAKE) run19200
	$(MAKE) run38400
# ========================
# HELPER MACRO (REPEATED PATTERN)
# ========================
define RUN_SIZE
	$(DATA_GEN) $(1)

	@copy thePoints.dat $(ACO)\thePoints.dat >nul
	@copy thePoints.dat $(ASTAR)\thePoints.dat >nul
	@copy thePoints.dat $(DIJKSTRA)\thePoints.dat >nul
	@copy thePoints.dat $(BELLMAN)\thePoints.dat >nul

	@$(ACO)\$(ALGO_EXE) $(ACO)\thePoints.dat
	@move /Y $(ACO)\*.txt $(ACO)\ >nul

	@$(ASTAR)\$(ALGO_EXE) $(ASTAR)\thePoints.dat
	@move /Y $(ASTAR)\*.txt $(ASTAR)\ >nul

	@$(DIJKSTRA)\$(ALGO_EXE) $(DIJKSTRA)\thePoints.dat
	@move /Y $(DIJKSTRA)\*.txt $(DIJKSTRA)\ >nul

	@$(BELLMAN)\$(ALGO_EXE) $(BELLMAN)\thePoints.dat
	@move /Y $(BELLMAN)\*.txt $(BELLMAN)\ >nul
endef

# ========================
# SIZE TARGETS
# ========================
run300:
	$(call RUN_SIZE,300)

run600:
	$(call RUN_SIZE,600)

run1200:
	$(call RUN_SIZE,1200)

run2400:
	$(call RUN_SIZE,2400)

run4800:
	$(call RUN_SIZE,4800)

run9600:
	$(call RUN_SIZE,9600)

run19200:
	$(call RUN_SIZE,19200)

run38400:
	$(call RUN_SIZE,38400)


# ========================
# PYTHON
# ========================
#python:
	python script.py

# ========================
# CLEAN
# ========================
clean:
	@del $(DATA_GEN).exe 2>nul
	@del $(ACO)\$(ALGO_EXE).exe 2>nul
	@del $(ASTAR)\$(ALGO_EXE).exe 2>nul
	@del $(DIJKSTRA)\$(ALGO_EXE).exe 2>nul
	@del $(BELLMAN)\$(ALGO_EXE).exe 2>nul
