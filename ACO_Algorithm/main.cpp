#include <iostream>
#include <vector>
#include <string>
#include "graph.h"
#include "aco.h"
using namespace std;

//  main.cpp — Ant Colony Optimization shortest path finder
//  If no argument is given, the program looks for thePoints.dat
//  in the current directory (the default output of dataGeneration).
////references used: 
//https://www.geeksforgeeks.org/machine-learning/introduction-to-ant-colony-optimization/

int main(int argc, char* argv[]) {
    string dataFile = "thePoints.dat";
    if (argc >= 2) {
        dataFile = argv[1];
    }
 
    vector<Node> graph;
    try {
        graph = loadGraph(dataFile);
    } catch (const exception& e) {
        cerr << "Error loading graph: " << e.what() << "\n";
        cerr << "Make sure you have run dataGeneration first to produce "
             << dataFile << ".\n";
        return 1;
    }
 
    AcoParams params;
    params.numAnts= 30;
    params.numIterations= 200;
    params.phInfluence= 1.0;
    params.distInfluence= 3.0;
    params.rho= 0.1;
    params.Q= 100.0;
    params.initialPh= 1.0;
 
    AntColonyOptimization aco(graph, 0, 299, params);
    AcoResult result = aco.run();
 
    ofstream outFile("results.txt");
    if (!outFile.is_open()) {
        cerr << "Error: could not open results.txt for writing.\n";
        return 1;
    }
 
    outFile << "Graph loaded: " << graph.size() << " nodes.\n";
    outFile << "Start: node 0 at (" << graph[0].x << ", " << graph[0].y << ")\n";
    outFile << "Goal : node 299 at (" << graph[299].x << ", " << graph[299].y << ")\n\n";
 
    outFile << "ACO Parameters:\n"
            << "Ants per iteration: " << params.numAnts       << "\n"
            << "Iterations: " << params.numIterations  << "\n"
            << "phInfluence: " << params.phInfluence    << "\n"
            << "distInfluence: " << params.distInfluence  << "\n"
            << "Rho (evaporation): " << params.rho            << "\n"
            << "Q (deposit): " << params.Q              << "\n\n";
 
    outFile << "=== ACO Complete ===\n";
 
    if (result.path.empty()) {
        outFile << "No valid path from node 0 to node 299 was found.\n"
                << "This may indicate the graph is disconnected. "
                << "Try regenerating thePoints.dat.\n";
        return 1;
    }
 
    outFile << "Best path length (Manhattan distance): " << result.totalCost << "\n";
    outFile << "Path node count  : " << result.path.size() << " nodes\n\n";
 
    outFile << "Path (node index -> x,y):\n";
    for (size_t i = 0; i < result.path.size(); ++i) {
        int idx = result.path[i];
        outFile << "  [" << i << "] Node " << idx
                << " (" << graph[idx].x << ", " << graph[idx].y << ")\n";
    }
 
    outFile << "\nMinimum (direct Manhattan): 198\n";
 
    outFile.close();
    return 0;
}
