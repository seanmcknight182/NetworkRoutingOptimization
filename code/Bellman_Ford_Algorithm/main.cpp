#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <chrono>
#include <algorithm>

using namespace std;

// ===================== DATA STRUCTS =====================

struct Point {
    int x, y;
    vector<int> neighbors;
};

struct Edge {
    int src, dst, weight;
};

// ===================== UTIL =====================

int manhattanDist(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// ===================== GRAPH LOADING =====================

bool loadGraph(const string& filename, vector<Point>& points, vector<Edge>& edges) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file\n";
        return false;
    }

    points.clear();
    edges.clear();

    while (true) {
        Point p;
        int n1, n2, n3, n4;

        if (!(file >> p.x >> p.y >> n1 >> n2 >> n3 >> n4))
            break;

        if (n1 != -1) p.neighbors.push_back(n1);
        if (n2 != -1) p.neighbors.push_back(n2);
        if (n3 != -1) p.neighbors.push_back(n3);
        if (n4 != -1) p.neighbors.push_back(n4);

        points.push_back(p);
    }

    for (int i = 0; i < (int)points.size(); i++) {
        for (int j : points[i].neighbors) {
            int w = manhattanDist(points[i], points[j]);
            edges.push_back({i, j, w});
        }
    }

    return true;
}

// ===================== BELLMAN-FORD =====================

int bellmanFord(const vector<Edge>& edges,
                int V,
                int src,
                vector<int>& dist,
                vector<int>& pred,
                long long& ops)
{
    dist.assign(V, INT_MAX);
    pred.assign(V, -1);

    dist[src] = 0;

    for (int i = 0; i < V - 1; i++) {
        bool updated = false;

        for (const Edge& e : edges) {
            ops++;

            if (dist[e.src] == INT_MAX) continue;

            int nd = dist[e.src] + e.weight;

            if (nd < dist[e.dst]) {
                dist[e.dst] = nd;
                pred[e.dst] = e.src;
                updated = true;
                ops++;
            }
        }

        if (!updated) break;
    }

    return 0;
}

// ===================== MAIN =====================

int main() {
    vector<Point> points;
    vector<Edge> edges;

    if (!loadGraph("thePoints.dat", points, edges))
        return 1;

    int n = points.size();

    int SRC = 0;
    int DST = n - 1;

    vector<int> dist, pred;
    long long ops = 0;

    // ⏱️ timing start
    auto t1 = chrono::high_resolution_clock::now();

    bellmanFord(edges, n, SRC, dist, pred, ops);

    // ⏱️ timing end
    auto t2 = chrono::high_resolution_clock::now();

    long long duration =
        chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();

    int weight = dist[DST];

    // ===================== OUTPUT =====================

    string filename = "3_" + to_string(n) + ".txt";
    ofstream out(filename);

    if (!out) {
        cerr << "Error writing output file\n";
        return 1;
    }

    // STRICT FORMAT ONLY
    out << "time_ms,basic_op_count,weight\n";
    out << duration << "," << ops << "," << weight << "\n";

    out.close();

    cout << "Done. Results written to " << filename << "\n";

    return 0;
}
