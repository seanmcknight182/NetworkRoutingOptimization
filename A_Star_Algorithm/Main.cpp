/****************************************************************************************
    Filename: Main.cpp
    Date: 04/20/26
    Authors: ChatGPT and Eli DeSha
    Purpose: Uses the data generated from dataGeneration.cpp and runs it through the A*
             alg to find the shortest path from the first node in the file to the last
             node in the file. The program then generates a results file indicating the
             shortest path and the total cost to get there.
****************************************************************************************/

#include <bits/stdc++.h>
using namespace std;

// Represents a point (node) in the graph
struct Node {
    int id;
    double x, y;
};

// Represents an edge between nodes
struct Edge {
    int to;          // destination node index
    double weight;   // cost to travel
};

// Graph is an adjacency list: each node has a list of edges
using Graph = vector<vector<Edge>>;

// Heuristic function (straight-line distance between two nodes)
// Used by A* to estimate distance to goal
double heuristic(const Node& a, const Node& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// A* pathfinding algorithm
pair<vector<int>, double> astar(Graph& graph, vector<Node>& nodes, int start, int goal) {
    int n = graph.size();

    // g = cost from start to node
    // f = g + heuristic estimate to goal
    vector<double> g(n, 1e18);
    vector<double> f(n, 1e18);

    // used to reconstruct path
    vector<int> parent(n, -1);

    // priority queue (min-heap based on f score)
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

    // initialize start node
    g[start] = 0;
    f[start] = heuristic(nodes[start], nodes[goal]);
    pq.push({f[start], start});

    // main A* loop
    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();

        // if we reached the goal, reconstruct path
        if (current == goal) {
            vector<int> path;

            for (int v = goal; v != -1; v = parent[v])
                path.push_back(v);

            reverse(path.begin(), path.end());
            return {path, g[goal]};
        }

        // explore neighbors
        for (auto &edge : graph[current]) {
            int nb = edge.to;

            // cost to reach neighbor through current node
            double tentative = g[current] + edge.weight;

            // if this path is better, update it
            if (tentative < g[nb]) {
                g[nb] = tentative;
                parent[nb] = current;

                // f = actual cost + estimated remaining cost
                f[nb] = tentative + heuristic(nodes[nb], nodes[goal]);

                pq.push({f[nb], nb});
            }
        }
    }

    // no path found
    return {{}, -1};
}

int main() {
    ifstream file("thePoints.dat");   // input graph data
    ofstream out("results.txt");      // output results

    // check file opened correctly
    if (!file) {
        cout << "Error opening input file\n";
        return 1;
    }

    if (!out) {
        cout << "Error creating output file\n";
        return 1;
    }

    vector<Node> nodes;                  // list of nodes
    vector<vector<int>> rawConnections;  // temporary edge list

    // PASS 1: read all nodes and raw connections from file
    while (true) {
        int x, y, t1, t2, t3, t4;

        if (!(file >> x >> y >> t1 >> t2 >> t3 >> t4))
            break;

        Node n;
        n.id = nodes.size();
        n.x = x;
        n.y = y;

        nodes.push_back(n);
        rawConnections.push_back({t1, t2, t3, t4});
    }

    file.close();

    // PASS 2: build adjacency list graph
    Graph graph(nodes.size());

    for (int i = 0; i < nodes.size(); i++) {
        for (int nb : rawConnections[i]) {

            // ignore invalid connections
            if (nb >= 0 && nb < nodes.size()) {

                // compute Euclidean distance as edge weight
                double dx = nodes[i].x - nodes[nb].x;
                double dy = nodes[i].y - nodes[nb].y;
                double dist = sqrt(dx*dx + dy*dy);

                graph[i].push_back({nb, dist});
            }
        }
    }

    int start = 0;                     // start node
    int goal = nodes.size() - 1;      // goal node

    // run A* search
    auto [path, cost] = astar(graph, nodes, start, goal);

    // write results to file
    if (path.empty()) {
        out << "No path found\n";
    } else {
        out << "Path:\n";

        for (int v : path)
            out << v << " ";

        out << "\nTotal cost: " << cost << "\n";
    }

    out.close();

    cout << "Done. Results written to results.txt\n";

    return 0;
}