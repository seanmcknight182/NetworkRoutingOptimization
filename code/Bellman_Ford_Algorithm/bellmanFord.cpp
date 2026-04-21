#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <chrono>
#include <algorithm>
using namespace std;

struct Point {
    int x, y;
    vector<int> neighbors; // dynamic instead of fixed 4
};

struct Edge {
    int src, dst, weight;
};

// Manhattan distance
int manhattanDist(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Load graph from file
bool loadGraph(const string& filename, vector<Point>& points, vector<Edge>& edges) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << "\n";
        return false;
    }

    points.clear();

    // Read until EOF (no hardcoded 300)
    while (true) {
        Point p;
        int n1, n2, n3, n4;

        if (!(file >> p.x >> p.y >> n1 >> n2 >> n3 >> n4)) break;

        p.neighbors.clear();
        if (n1 != -1) p.neighbors.push_back(n1);
        if (n2 != -1) p.neighbors.push_back(n2);
        if (n3 != -1) p.neighbors.push_back(n3);
        if (n4 != -1) p.neighbors.push_back(n4);

        points.push_back(p);
    }

    // Build edges
    for (int i = 0; i < (int)points.size(); i++) {
        for (int j : points[i].neighbors) {
            int w = manhattanDist(points[i], points[j]);
            edges.push_back({i, j, w});
        }
    }

    return true;
}

// Bellman-Ford
bool bellmanFord(const vector<Point>& points,
                 const vector<Edge>& edges,
                 int src,
                 vector<int>& dist,
                 vector<int>& pred)
{
    int V = points.size();
    dist.assign(V, INT_MAX);
    pred.assign(V, -1);
    dist[src] = 0;

    for (int i = 0; i < V - 1; i++) {
        bool updated = false;
        for (const Edge& e : edges) {
            if (dist[e.src] == INT_MAX) continue;

            int nd = dist[e.src] + e.weight;
            if (nd < dist[e.dst]) {
                dist[e.dst] = nd;
                pred[e.dst] = e.src;
                updated = true;
            }
        }
        if (!updated) break;
    }

    return true;
}

// Path reconstruction
vector<int> reconstructPath(const vector<int>& pred, int src, int dst) {
    vector<int> path;
    for (int cur = dst; cur != -1; cur = pred[cur]) {
        path.push_back(cur);
        if (cur == src) break;
    }
    reverse(path.begin(), path.end());

    if (path.empty() || path[0] != src) return {};
    return path;
}

int main() {
    vector<Point> points;
    vector<Edge> edges;

    if (!loadGraph("thePoints.dat", points, edges)) return 1;

    int SRC = 0;
    int DST = points.size() - 1;

    vector<int> dist, pred;

    auto start = chrono::high_resolution_clock::now();
    bellmanFord(points, edges, SRC, dist, pred);
    auto end = chrono::high_resolution_clock::now();

    double ms = chrono::duration<double, milli>(end - start).count();

    cout << "Distance: " << dist[DST] << "\n";
    cout << "Runtime: " << ms << " ms\n";

    vector<int> path = reconstructPath(pred, SRC, DST);

    cout << "Path:\n";
    for (int i = 0; i < (int)path.size(); i++) {
        int idx = path[i];
        cout << idx << " (" << points[idx].x << "," << points[idx].y << ")";
        if (i + 1 < (int)path.size()) {
            int nxt = path[i + 1];
            cout << " -> ";
        }
        cout << "\n";
    }

    return 0;
}
