#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <queue>

// ChatGPT was a big help

using namespace std;

// This was much easier than the data generation, translating from psuedo code is basically a nothing burger task.

const int N = 300;

struct Point {
	int x, y;
};

struct Edge {
	int to;
	int weight;
};

int Distance( Point& a, Point& b) {	// because the points dont link on diagonals this is manhatten distance. Euclid can get lost!
 	return abs(a.x - b.x) + abs(a.y - b.y);
}
	
Point points[N];
Edge graph[N][N];     // worst-case adjacency
int degree[N] = {0};   // number of edges per node

int main() {
	ifstream file("thePoints.dat");
    	if (!file) {
        	cerr << "Could not open file\n";
        	return 1;
    	}

	string line;

   	// reading the file
   	for (int i = 0; i < N; i++) {
        	getline(file, line);
        	stringstream ss(line);	// naming this variable that was probabbly a poor descision.

        	ss >> points[i].x >> points[i].y;

       		int neighbor;
        	while (ss >> neighbor) {
        	    graph[i][degree[i]++] = {neighbor, 0};
        	}
    	}

	// and now we compute the weights
    	for (int i = 0; i < N; i++) {
        	for (int j = 0; j < degree[i]; j++) {
       	     		int v = graph[i][j].to;
            		graph[i][j].weight = Distance(points[i], points[v]);
		}
	}

    	// The algorithnm itself, the meat and the potatos if you will; God I love potatos. THANK YOU INDIGNOUS SOUTH AMERICANS.  
	int start = 0;
	int dist[N];
	int parent[N];
    	for (int i = 0; i < N; i++) {
        dist[i] = numeric_limits<int>::max();
        parent[i] = -1;
    	}
    	using P = pair<int,int>;
    	priority_queue<P, vector<P>, greater<P>> pq;
    	dist[start] = 0;
    	pq.push({0, start});
    	while (!pq.empty()) {
        	auto [d, u] = pq.top();	// Danke Vater, apparently this is more optimized or something, to be frank(charlgemaine is that you??), i dont know what makes this better.
        	pq.pop();

        if (d > dist[u]) continue;

 		for (int i = 0; i < degree[u]; i++) {
            		int v = graph[u][i].to;
            		int w = graph[u][i].weight;

            		int nd = d + w;

            		if (nd < dist[v]) {
                		dist[v] = nd;
                		parent[v] = u;
                		pq.push({nd, v});
            		}
        	}
    	}


    return 0;
}
