# Ant Colony Optimization — Shortest Path

**CSC2400 Group Project | Problem 6**  
Erick Ramirez · Eli DeSha · Sean McKnight

---

## What Is Ant Colony Optimization?

Ant Colony Optimization (ACO) is a **nature-inspired, probabilistic algorithm** that mimics how real ant colonies find food. In nature, ants wander randomly at first, but leave behind a chemical trail called **pheromone** as they walk. Shorter paths between the nest and food are traveled more frequently, so pheromone builds up faster on them. Eventually, almost all ants converge on the shortest route.

In the algorithm:
- **Nodes** = routers / intersections / waypoints
- **Edges** = roads / network connections (each with a travel cost/weight)
- **Ants** = software agents that traverse the graph
- **Pheromone** = a numeric value stored on each edge that reflects how "attractive" that edge has been proven to be

---

## How the Algorithm Works (Step by Step)

```
1. Initialize pheromone on every edge to the same small value.

2. For each ITERATION:
   a. Send out N ants from the source node.
   b. Each ant independently walks to the destination:
        - At every node, the ant probabilistically picks the next neighbor.
        - Probability is proportional to:
              pheromone^alpha  *  (1 / edge_weight)^beta
        - Higher pheromone = more attractive.
        - Lower weight = more attractive.
   c. After each ant finishes its walk, it deposits pheromone:
              deposit = 1 / total_path_cost
        - Shorter paths → larger deposit → stronger trail.
   d. After all ants are done, evaporate all pheromones:
              pheromone *= (1 - evaporation_rate)
        - This prevents old, suboptimal trails from dominating forever.

3. Track the globally best path found across all iterations.

4. Return best path, its total cost, and which iteration found it.
```

---

## Key Parameters and What They Do

| Parameter | What It Controls | Effect of Increasing |
|---|---|---|
| `num_ants` | How many ants explore per iteration | More exploration, slower per iteration |
| `num_iterations` | Total rounds of exploration | More time to converge |
| `alpha` | Pheromone influence | Ants follow trails more strictly |
| `beta` | Distance/heuristic influence | Ants prefer shorter edges |
| `evaporation_rate` | How fast pheromone fades | Faster forgetting, more exploration |
| `initial_pheromone` | Starting pheromone on all edges | Affects early randomness |

**Recommended starting values:** `alpha=1.0`, `beta=2-3`, `evaporation_rate=0.4-0.5`

---

## Project File Structure

```
aco_shortest_path/
├── main.py          ← Run this. Two demos: small known graph + random graph.
├── aco.py           ← The ACO algorithm (core logic).
├── graph_utils.py   ← Graph generation, save/load, and inspection tools.
├── reporter.py      ← Result formatting and ASCII convergence chart.
└── README.md        ← This file.
```

### Module Responsibilities

- **`aco.py`** — `AntColonyOptimization` class. Handles pheromone init, ant movement, pheromone evaporation/deposit, and the main solve loop. No I/O.
- **`graph_utils.py`** — Pure graph utilities: generate random graphs, build manual graphs from edge lists, save/load graphs as JSON, print summaries.
- **`reporter.py`** — Print results and convergence charts to stdout. No algorithm logic.
- **`main.py`** — Wires everything together. Only file you need to run.

---

## How to Run

```bash
# No dependencies beyond Python 3.10+
python main.py
```

You should see output for two demos:
1. **Small known graph** — verifies the algorithm finds the correct optimal path (A→C→E, cost 5)
2. **Random 30-node graph** — shows ACO on a larger generated graph

---

## Strengths and Weaknesses (per project spec)

**Strengths**
- Handles problems that are impractical for deterministic algorithms (e.g. Traveling Salesman)
- Naturally parallelizable (each ant is independent)
- Does not require the full graph to be explored exhaustively

**Weaknesses**
- **Probabilistic** — may return a suboptimal solution on any given run
- **No guarantee** of finding the global optimum
- Time complexity per iteration is approximately **O(V²)** for a fully connected graph (each of N ants visits up to V nodes), with V = number of vertices
- Parameter tuning (`alpha`, `beta`, `evaporation_rate`) requires experimentation

---

## Comparison with Other Algorithms in This Project

| Algorithm | Time Complexity | Finds Optimal? | Negative Weights |
|---|---|---|---|
| Dijkstra | O((V+E) log V) | Yes | No |
| Bellman-Ford | O(VE) | Yes | Yes |
| A* | O((V+E) log V) | Yes (with admissible heuristic) | No |
| **ACO** | **~O(V² × ants × iterations)** | **Probabilistic** | Possible with modifications |

---

## Extending This Code

To use your own graph from OpenStreetMap data, build a graph dict in the format:
```python
graph = {
    "NodeA": [("NodeB", 5.2), ("NodeC", 3.1)],
    "NodeB": [("NodeA", 5.2), ("NodeC", 2.0)],
    ...
}
```
Then pass it directly into `AntColonyOptimization(graph, ...)` and call `.solve(source, destination)`.
