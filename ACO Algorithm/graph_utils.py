"""
graph_utils.py
--------------
Utilities for generating, loading, and inspecting graphs used by the ACO algorithm.
All graphs are represented as adjacency lists:
    { node_name: [(neighbor_name, weight), ...], ... }
"""

import random
import json
import os


def generate_random_graph(num_nodes: int, edge_density: float = 0.3, weight_range: tuple = (1, 20), seed: int = None) -> dict:
    """
    Generate a random undirected weighted graph.

    Parameters
    ----------
    num_nodes : int
        Number of nodes in the graph.
    edge_density : float
        Probability that any two nodes are connected (0.0 to 1.0).
    weight_range : tuple
        (min_weight, max_weight) for random edge weights.
    seed : int, optional
        Random seed for reproducibility.

    Returns
    -------
    dict : adjacency list graph
    """
    if seed is not None:
        random.seed(seed)

    nodes = [f"N{i}" for i in range(num_nodes)]
    graph = {node: [] for node in nodes}

    # Guarantee connectivity: build a spanning chain first
    shuffled = nodes[:]
    random.shuffle(shuffled)
    for i in range(len(shuffled) - 1):
        a, b = shuffled[i], shuffled[i + 1]
        weight = random.randint(*weight_range)
        graph[a].append((b, weight))
        graph[b].append((a, weight))

    # Add random additional edges based on density
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            if random.random() < edge_density:
                a, b = nodes[i], nodes[j]
                # Avoid duplicate edges
                existing_neighbors = [n for n, _ in graph[a]]
                if b not in existing_neighbors:
                    weight = random.randint(*weight_range)
                    graph[a].append((b, weight))
                    graph[b].append((a, weight))

    return graph


def save_graph(graph: dict, filepath: str):
    """
    Save a graph to a JSON file.

    Parameters
    ----------
    graph : dict
        Adjacency list graph.
    filepath : str
        Destination file path (e.g., 'graphs/small.json').
    """
    os.makedirs(os.path.dirname(filepath) or ".", exist_ok=True)
    serializable = {node: neighbors for node, neighbors in graph.items()}
    with open(filepath, "w") as f:
        json.dump(serializable, f, indent=2)
    print(f"Graph saved to {filepath}")


def load_graph(filepath: str) -> dict:
    """
    Load a graph from a JSON file.

    Parameters
    ----------
    filepath : str
        Path to a JSON graph file produced by save_graph().

    Returns
    -------
    dict : adjacency list graph
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Graph file not found: {filepath}")

    with open(filepath, "r") as f:
        raw = json.load(f)

    # Convert lists-of-lists back to lists-of-tuples
    graph = {node: [tuple(e) for e in neighbors] for node, neighbors in raw.items()}
    return graph


def graph_summary(graph: dict) -> dict:
    """
    Return basic statistics about a graph.

    Returns
    -------
    dict with keys: num_nodes, num_edges, avg_degree, nodes (list)
    """
    num_nodes = len(graph)
    total_degree = sum(len(neighbors) for neighbors in graph.values())
    num_edges = total_degree // 2  # undirected: each edge counted twice

    return {
        "num_nodes": num_nodes,
        "num_edges": num_edges,
        "avg_degree": round(total_degree / num_nodes, 2) if num_nodes else 0,
        "nodes": list(graph.keys()),
    }


def build_manual_graph(edges: list) -> dict:
    """
    Build a graph from a list of (node_a, node_b, weight) tuples.

    Parameters
    ----------
    edges : list of (str, str, float)
        Each tuple defines an undirected edge and its weight.

    Returns
    -------
    dict : adjacency list graph
    """
    graph = {}
    for node_a, node_b, weight in edges:
        graph.setdefault(node_a, []).append((node_b, weight))
        graph.setdefault(node_b, []).append((node_a, weight))
    return graph
