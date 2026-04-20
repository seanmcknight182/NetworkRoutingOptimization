#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
using namespace std;

struct Node {
    int x, y;
    vector<int> neighbors; // indices of adjacent nodes (up to 4)
};

//returns Manhattan distance between two nodes
inline double manhattanDist(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

inline vector<Node> loadGraph(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Cannot open graph file: " + filePath);
    }

    vector<Node> graph;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        Node node;
        int n1, n2, n3, n4;

        if (!(iss >> node.x >> node.y >> n1 >> n2 >> n3 >> n4)) {
            throw runtime_error("Malformed line in graph file: " + line);
        }

        //only add valid (non -1) neighbors
        for (int n : {n1, n2, n3, n4}) {
            if (n != -1) node.neighbors.push_back(n);
        }

        graph.push_back(node);
    }

    if (graph.empty()) {
        throw runtime_error("Graph file is empty or could not be parsed.");
    }

    return graph;
}
