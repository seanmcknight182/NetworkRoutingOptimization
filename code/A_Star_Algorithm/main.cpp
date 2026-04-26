/****************************************************************************************
    Filename: Main.cpp
    Date: 04/20/26
    Authors: ChatGPT and Eli DeSha
    Purpose: Uses the data generated from dataGeneration.cpp and runs it through the A*
             algorithm to find the shortest path from the first node in the file to the last
             node in the file. The program then generates a results file indicating the
             shortest path and the total cost to get there.
****************************************************************************************/

#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

// Algorithm ID for A*
const int ALGO_ID = 1;

// Node structure
struct Node {
    int id;
    int x, y;
};

// Edge structure
struct Edge {
    int to;
    int weight;
};

using Graph = vector<vector<Edge>>;

// Manhattan heuristic
double heuristic(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// A* with operation counting
tuple<double, long long> astar(Graph& graph, vector<Node>& nodes, int start, int goal) {
    int n = graph.size();

    vector<double> g(n, 1e18);
    vector<double> f(n, 1e18);
    vector<int> parent(n, -1);

    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

    long long ops = 0;

    g[start] = 0;
    f[start] = heuristic(nodes[start], nodes[goal]);
    pq.push({f[start], start});

    while (!pq.empty()) {
        ops++;

        int current = pq.top().second;
        pq.pop();

        if (current == goal) {
            return {g[goal], ops};
        }

        for (auto &edge : graph[current]) {
            ops++;

            int nb = edge.to;
            double tentative = g[current] + edge.weight;

            if (tentative < g[nb]) {
                ops++;

                g[nb] = tentative;
                parent[nb] = current;
                f[nb] = tentative + heuristic(nodes[nb], nodes[goal]);

                pq.push({f[nb], nb});
            }
        }
    }

    return {-1, ops};
}

int main() {
    ifstream file("thePoints.dat");

    if (!file) {
        cout << "Error opening input file\n";
        return 1;
    }

    vector<Node> nodes;
    vector<vector<int>> rawConnections;

    string line;

    // ✅ SKIP FIRST LINE (important fix)
    getline(file, line);

    // Read graph data
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

    // Build graph using Manhattan distance
    Graph graph(nodes.size());

    for (int i = 0; i < (int)nodes.size(); i++) {
        for (int nb : rawConnections[i]) {
            if (nb >= 0 && nb < (int)nodes.size()) {
                int dist = abs(nodes[i].x - nodes[nb].x) +
                           abs(nodes[i].y - nodes[nb].y);

                graph[i].push_back({nb, dist});
            }
        }
    }

    int start = 0;
    int goal = nodes.size() - 1;

    // Timing
    auto t1 = high_resolution_clock::now();
    auto [cost, ops] = astar(graph, nodes, start, goal);
    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(t2 - t1).count();
    double time_ms = duration / 1000.0;

    // Output file format: algoID_numPoints.txt
    string filename = to_string(ALGO_ID) + "_" + to_string(nodes.size()) + ".txt";
    ofstream out(filename);

    if (!out) {
        cout << "Error creating output file\n";
        return 1;
    }

    // STRICT OUTPUT FORMAT
    out << "time_ms,basic_op_count,weight\n";
    out << fixed << setprecision(3);
    out << time_ms << "," << ops << "," << cost << "\n";

    out.close();

    cout << "Done. Results written to " << filename << "\n";

    return 0;
}
