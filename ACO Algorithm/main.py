"""
main.py
-------
Entry point for running the Ant Colony Optimization shortest path algorithm.

Usage
-----
    python main.py

No manual editing required. The script demonstrates ACO on two scenarios:
    1. A small hand-crafted graph (verifiable by inspection)
    2. A randomly generated graph of configurable size
"""

from aco import AntColonyOptimization
from graph_utils import generate_random_graph, build_manual_graph, graph_summary
from reporter import print_result, print_convergence

# ACO Configuration
ACO_CONFIG = {
    "num_ants": 30,           # ants per iteration
    "num_iterations": 150,    # total iterations
    "alpha": 1.0,             # pheromone influence
    "beta": 3.0,              # distance influence
    "evaporation_rate": 0.4,  # pheromone decay per iteration
    "initial_pheromone": 1.0, # starting pheromone on every edge
}

# Demo 1: Small known graph
def demo_small_graph():
    """
    Run ACO on a small graph whose optimal path is known.
    Optimal path: A → C → E  (cost = 2 + 3 = 5)
    """
    print("\n" + "=" * 55)
    print("  DEMO 1: Small Known Graph")
    print("=" * 55)

    edges = [
        ("A", "B", 4),
        ("A", "C", 2),
        ("B", "D", 5),
        ("B", "E", 11),
        ("C", "B", 1),
        ("C", "D", 8),
        ("C", "E", 3),    # A→C→E = 2+3 = 5  ← optimal
        ("D", "E", 2),
        ("E", "F", 6),
    ]

    graph = build_manual_graph(edges)
    summary = graph_summary(graph)
    print(f"\n  Graph: {summary['num_nodes']} nodes, {summary['num_edges']} edges")

    aco = AntColonyOptimization(graph, **ACO_CONFIG)
    result = aco.solve(source="A", destination="E")

    print_result(result, source="A", destination="E")
    print_convergence(result["history"])

    print("  Expected optimal path : A → C → E  (cost 5)")
    print()

# Demo 2: Randomly generated graph
def demo_random_graph(num_nodes: int = 30, seed: int = 42):
    """
    Run ACO on a randomly generated graph.

    Parameters
    ----------
    num_nodes : int
        Size of the random graph.
    seed : int
        Random seed (keeps results reproducible across runs).
    """
    print("\n" + "=" * 55)
    print(f"  DEMO 2: Random Graph ({num_nodes} nodes)")
    print("=" * 55)

    graph = generate_random_graph(
        num_nodes=num_nodes,
        edge_density=0.25,
        weight_range=(1, 20),
        seed=seed,
    )

    summary = graph_summary(graph)
    nodes = summary["nodes"]
    source, destination = nodes[0], nodes[-1]

    print(f"\n  Graph   : {summary['num_nodes']} nodes, {summary['num_edges']} edges")
    print(f"  Route   : {source}  →  {destination}")
    print(f"  Avg deg : {summary['avg_degree']}")

    aco = AntColonyOptimization(graph, **ACO_CONFIG)
    result = aco.solve(source=source, destination=destination)

    print_result(result, source=source, destination=destination)
    print_convergence(result["history"])


#Starting Point
if __name__ == "__main__":
    demo_small_graph()
    demo_random_graph(num_nodes=30, seed=42)
