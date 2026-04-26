#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

// ===================== GRAPH =====================

struct Node {
    int x, y;
    vector<int> neighbors;
};

inline double manhattanDist(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

vector<Node> loadGraph(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Cannot open graph file: " + filePath);
    }

    vector<Node> graph;
    string line;

    // skip first line (N)
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        Node node;
        int n1, n2, n3, n4;

        if (!(iss >> node.x >> node.y >> n1 >> n2 >> n3 >> n4)) {
            throw runtime_error("Malformed line: " + line);
        }

        for (int n : {n1, n2, n3, n4}) {
            if (n != -1) node.neighbors.push_back(n);
        }

        graph.push_back(node);
    }

    return graph;
}

// ===================== ACO =====================

struct AcoParams {
    int numAnts = 30;
    int numIterations = 200;
    double phInfluence = 1.0;
    double distInfluence = 3.0;
    double rho = 0.1;
    double Q = 100.0;
    double initialPh = 1.0;
};

struct AcoResult {
    vector<int> path;
    int totalCost = 0;
};

class AntColonyOptimization {
public:
    AntColonyOptimization(const vector<Node>& graph, int startNode, int goalNode, const AcoParams& params)
        : graph_(graph), start_(startNode), goal_(goalNode), params_(params), rng_(random_device{}())
    {
        initPheromones();
    }

    AcoResult run(long long& ops) {
        AcoResult best;
        best.totalCost = numeric_limits<int>::max();

        for (int iter = 0; iter < params_.numIterations; ++iter) {

            vector<vector<int>> allPaths;
            vector<int> allCosts;

            for (int ant = 0; ant < params_.numAnts; ++ant) {
                vector<int> path;
                double cost = 0.0;

                ops++;

                if (constructTour(path, cost, ops)) {
                    int icost = (int)cost;
                    allPaths.push_back(path);
                    allCosts.push_back(icost);

                    if (icost < best.totalCost) {
                        best.totalCost = icost;
                        best.path = path;
                    }
                }
            }

            evaporate();
            depositPheromones(allPaths, allCosts, ops);
        }

        return best;
    }

private:
    const vector<Node>& graph_;
    int start_, goal_;
    AcoParams params_;
    mt19937 rng_;
    vector<vector<double>> pheromone_;

    void initPheromones() {
        pheromone_.resize(graph_.size());
        for (size_t u = 0; u < graph_.size(); ++u) {
            pheromone_[u].assign(graph_[u].neighbors.size(), params_.initialPh);
        }
    }

    bool constructTour(vector<int>& path, double& cost, long long& ops) {
        vector<bool> visited(graph_.size(), false);

        int current = start_;
        path.clear();
        cost = 0;

        path.push_back(current);
        visited[current] = true;

        int maxSteps = graph_.size() * 2;

        for (int step = 0; step < maxSteps; ++step) {
            ops++;

            if (current == goal_) return true;

            int next = chooseNext(current, visited, ops);
            if (next == -1) return false;

            cost += manhattanDist(graph_[current], graph_[next]);

            visited[next] = true;
            path.push_back(next);
            current = next;
        }

        return current == goal_;
    }

    int chooseNext(int current, const vector<bool>& visited, long long& ops) {
        const auto& neighbors = graph_[current].neighbors;
        if (neighbors.empty()) return -1;

        vector<double> weights;
        vector<int> candidates;

        for (size_t i = 0; i < neighbors.size(); ++i) {
            int v = neighbors[i];
            if (visited[v]) continue;

            ops++;

            double dist = manhattanDist(graph_[current], graph_[v]);
            if (dist <= 0) dist = 1e-9;

            double tau = pheromone_[current][i];
            double eta = 1.0 / dist;

            double weight = pow(tau, params_.phInfluence) *
                            pow(eta, params_.distInfluence);

            candidates.push_back(v);
            weights.push_back(weight);
        }

        if (candidates.empty()) return -1;

        discrete_distribution<int> dist(weights.begin(), weights.end());
        return candidates[dist(rng_)];
    }

    void evaporate() {
        for (auto& row : pheromone_) {
            for (double& tau : row) {
                tau *= (1.0 - params_.rho);
                if (tau < 1e-9) tau = 1e-9;
            }
        }
    }

    void depositPheromones(const vector<vector<int>>& paths,
                           const vector<int>& costs,
                           long long& ops) {
        for (size_t a = 0; a < paths.size(); ++a) {
            if (costs[a] <= 0) continue;

            double deposit = params_.Q / costs[a];
            const auto& path = paths[a];

            for (size_t i = 0; i + 1 < path.size(); ++i) {
                ops++;

                int u = path[i];
                int v = path[i + 1];

                const auto& nbrs = graph_[u].neighbors;

                for (size_t j = 0; j < nbrs.size(); ++j) {
                    if (nbrs[j] == v) {
                        pheromone_[u][j] += deposit;
                        break;
                    }
                }
            }
        }
    }
};

// ===================== MAIN =====================

int main(int argc, char* argv[]) {
    string dataFile = "thePoints.dat";
    if (argc >= 2) dataFile = argv[1];

    vector<Node> graph;

    try {
        graph = loadGraph(dataFile);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    int n = graph.size();

    AcoParams params;
    AntColonyOptimization aco(graph, 0, n - 1, params);

    long long ops = 0;

    auto t1 = chrono::high_resolution_clock::now();
    AcoResult result = aco.run(ops);
    auto t2 = chrono::high_resolution_clock::now();

    // ✅ FIXED: floating-point milliseconds
    chrono::duration<double, milli> duration = t2 - t1;
    double time_ms = duration.count();

    string filename = "2_" + to_string(n) + ".txt";
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cerr << "Error: could not open output file\n";
        return 1;
    }

    outFile << "time_ms,basic_op_count,weight\n";
    outFile << time_ms << "," << ops << "," << result.totalCost << "\n";

    outFile.close();

    cout << "Done. Results written to " << filename << "\n";

    return 0;
}
