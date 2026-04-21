#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

//graphs

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

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        Node node;
        int n1, n2, n3, n4;

        if (!(iss >> node.x >> node.y >> n1 >> n2 >> n3 >> n4)) {
            throw runtime_error("Malformed line in graph file: " + line);
        }

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

//aco

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
    double totalCost;
    int iterations;
};

class AntColonyOptimization {
public:
    AntColonyOptimization(const vector<Node>& graph, int startNode, int goalNode, const AcoParams& params = AcoParams{})
        : graph_(graph), start_(startNode), goal_(goalNode), params_(params), rng_(random_device{}())
    {
        initPheromones();
    }

    AcoResult run() {
        AcoResult best;
        best.totalCost = numeric_limits<double>::infinity();
        best.iterations = params_.numIterations;

        for (int iter = 0; iter < params_.numIterations; ++iter) {

            vector<vector<int>> allPaths;
            vector<double> allCosts;

            for (int ant = 0; ant < params_.numAnts; ++ant) {
                vector<int> path;
                double cost = 0.0;

                if (constructTour(path, cost)) {
                    allPaths.push_back(path);
                    allCosts.push_back(cost);

                    if (cost < best.totalCost) {
                        best.totalCost = cost;
                        best.path = path;
                    }
                }
            }

            evaporate();
            depositPheromones(allPaths, allCosts);
        }

        return best;
    }

private:
    const vector<Node>& graph_;
    int start_;
    int goal_;
    AcoParams params_;
    mt19937 rng_;
    vector<vector<double>> pheromone_;

    void initPheromones() {
        pheromone_.resize(graph_.size());
        for (size_t u = 0; u < graph_.size(); ++u) {
            pheromone_[u].assign(graph_[u].neighbors.size(), params_.initialPh);
        }
    }

    bool constructTour(vector<int>& path, double& cost) {
        vector<bool> visited(graph_.size(), false);
        path.clear();
        cost = 0.0;

        int current = start_;
        path.push_back(current);
        visited[current] = true;

        const int maxSteps = static_cast<int>(graph_.size()) * 2;

        for (int step = 0; step < maxSteps; ++step) {
            if (current == goal_) return true;

            int next = chooseNext(current, visited);
            if (next == -1) return false;

            cost += manhattanDist(graph_[current], graph_[next]);
            visited[next] = true;
            path.push_back(next);
            current = next;
        }

        return (current == goal_);
    }

    int chooseNext(int current, const vector<bool>& visited) {
        const auto& neighbors = graph_[current].neighbors;
        if (neighbors.empty()) return -1;

        vector<double> weights;
        vector<int> candidates;

        for (size_t i = 0; i < neighbors.size(); ++i) {
            int v = neighbors[i];
            if (visited[v]) continue;

            double dist = manhattanDist(graph_[current], graph_[v]);
            if (dist <= 0.0) dist = 1e-9;

            double tau = pheromone_[current][i];
            double eta = 1.0 / dist;
            double weight = pow(tau, params_.phInfluence) * pow(eta, params_.distInfluence);

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

    void depositPheromones(const vector<vector<int>>& paths, const vector<double>& costs) {
        for (size_t a = 0; a < paths.size(); ++a) {
            double deposit = params_.Q / costs[a];
            const auto& path = paths[a];

            for (size_t step = 0; step + 1 < path.size(); ++step) {
                int u = path[step];
                int v = path[step + 1];

                const auto& nbrs = graph_[u].neighbors;
                for (size_t i = 0; i < nbrs.size(); ++i) {
                    if (nbrs[i] == v) {
                        pheromone_[u][i] += deposit;
                        break;
                    }
                }
            }
        }
    }
};

// MAIN

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
        return 1;
    }

    AcoParams params;

    AntColonyOptimization aco(graph, 0, 299, params);
    AcoResult result = aco.run();

    ofstream outFile("results.txt");
    if (!outFile.is_open()) {
        cerr << "Error: could not open results.txt\n";
        return 1;
    }

    outFile << "Best path length: " << result.totalCost << "\n";

    for (int idx : result.path) {
        outFile << idx << " (" << graph[idx].x << ", " << graph[idx].y << ")\n";
    }

    outFile.close();
    return 0;
}
