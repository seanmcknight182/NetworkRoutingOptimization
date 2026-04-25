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
using namespace chrono;

// Node structure
struct Node {
    int id;
    double x, y;
};

// Edge structure
struct Edge {
    int to;
    double weight;
};

using Graph = vector<vector<Edge>>;

// Heuristic (Euclidean distance)
double heuristic(const Node& a, const Node& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// A* with operation counting
tuple<vector<int>, double, long long> astar(Graph& graph, vector<Node>& nodes, int start, int goal) {
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
            vector<int> path;

            for (int v = goal; v != -1; v = parent[v]) {
                path.push_back(v);
                ops++;
            }

            reverse(path.begin(), path.end());
            return {path, g[goal], ops};
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

    return {{}, -1, ops};
}

int main() {
    ifstream file("thePoints.dat");

    if (!file) {
        cout << "Error opening input file\n";
        return 1;
    }

    vector<Node> nodes;
    vector<vector<int>> rawConnections;

    // Read input
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

    // Build graph
    Graph graph(nodes.size());

    for (int i = 0; i < nodes.size(); i++) {
        for (int nb : rawConnections[i]) {
            if (nb >= 0 && nb < nodes.size()) {
                double dx = nodes[i].x - nodes[nb].x;
                double dy = nodes[i].y - nodes[nb].y;
                double dist = sqrt(dx*dx + dy*dy);

                graph[i].push_back({nb, dist});
            }
        }
    }

    int start = 0;
    int goal = nodes.size() - 1;

    // Timing start
    auto t1 = high_resolution_clock::now();

    auto [path, cost, ops] = astar(graph, nodes, start, goal);

    // Timing end
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    // Create dynamic filename
    string filename = "results_" + to_string(nodes.size()) + ".csv";
    ofstream out(filename);

    if (!out) {
        cout << "Error creating output file\n";
        return 1;
    }

    // Write CSV
    out << "time_ms,basic_op_count,weight\n";
    out << duration << "," << ops << "," << cost << "\n";

    out.close();

    cout << "Done. Results written to " << filename << "\n";

    return 0;
}
