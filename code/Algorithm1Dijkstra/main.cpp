#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
#include <string>
// had chatgpt refactor this, so technically the whole thing is the doing of chat gpt. We really are gonna be replaced by AI, arent we? God has abandoned the programmers for their
// apostacy, millions must install templeOS. Terry Davis was right.


using namespace std;

struct Point {
    int x, y;
};

struct Edge {
    int to;
    int weight;
};

int Distance(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int main() {
    ifstream file("thePoints.dat");
    if (!file) {
        cerr << "Could not open file\n";
        return 1;
    }

    int basicOPCount = 0;
    int totalWeight = 0;
    //int totalDollars = 1000000000; // please, I need money so bad.
    
    int N;
    file >> N;
	
    //cout << "N = " << N << "\n";
    auto time1 = chrono::high_resolution_clock::now();	//starting the clock here
    vector<Point> points(N);
    vector<vector<Edge>> graph(N);

    string line;
    getline(file, line); // consume leftover newline

    // Read points + adjacency lists
    for (int i = 0; i < N; i++) {
        getline(file, line);
        stringstream ss(line);

        ss >> points[i].x >> points[i].y;

        int neighbor;
        while (ss >> neighbor) {
            graph[i].push_back({neighbor, 0});
        }
    }

    file.close();

    // Compute edge weights
    for (int i = 0; i < N; i++) {
        for (auto &edge : graph[i]) {
            edge.weight = Distance(points[i], points[edge.to]);
        }
    }

    // Dijkstra setup
    int start = 0;
    vector<int> dist(N, numeric_limits<int>::max());
    vector<int> parent(N, -1);

    using P = pair<int, int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &edge : graph[u]) {
            int v = edge.to;
            int w = edge.weight;
	//	basicOPCount++; // im tweaking trying to figure out where to put this thing dude
            int nd = d + w;

            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }
	auto time2 = chrono::high_resolution_clock::now();	//ending the clock here
	auto completionTime = chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();	// in the voice of Smokey Bear "Only you can prevent namespace pollution!" 
											// God, c++ is such a stupid language, why did we write this in c++ when we could've done it in c
											// im not going insane at all, idk what you mean
    // Output results
    string theFileName= "0_";
    theFileName = theFileName + to_string(N);
    theFileName = theFileName + ".txt";
    ofstream outputfile(theFileName);

    int target = N - 1;


    	
    if (dist[target] == numeric_limits<int>::max()) {
        cout << "No path exists from " << start << " to " << target << ".\n";	// this realistically should never fire; the chud who wrote our data generator ensured this.
    } else {
        vector<int> path;
        for (int v = target; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
	outputfile << "time_ms,basic_op_count,weight" << "\n";
	outputfile << completionTime << "," << basicOPCount << "," << totalWeight;
        //outputfile << "Path:\n";
        //for (int v : path) {
        //    outputfile << v << " ";
        }
        outputfile << "\n";	
    	outputfile.close();
	return 0;
}
