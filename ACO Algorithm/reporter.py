"""
reporter.py
-----------
Handles printing and summarizing ACO results in a clean, readable format.
"""

import math


def print_result(result: dict, source: str, destination: str):
    """
    Print ACO result to the console in a formatted layout.

    Parameters
    ----------
    result : dict
        Output from AntColonyOptimization.solve()
    source : str
        Start node label.
    destination : str
        End node label.
    """
    sep = "-" * 50
    print(sep)
    print("  ACO SHORTEST PATH RESULT")
    print(sep)
    print(f"  From       : {source}")
    print(f"  To         : {destination}")

    if not result["best_path"] or result["best_cost"] == math.inf:
        print("  Status     : NO PATH FOUND")
    else:
        path_str = " → ".join(result["best_path"])
        print(f"  Best Path  : {path_str}")
        print(f"  Total Cost : {result['best_cost']:.4f}")
        print(f"  Found at   : Iteration {result['iteration'] + 1}")
        print(f"  Path Nodes : {len(result['best_path'])}")

    print(sep)


def convergence_summary(history: list) -> dict:
    """
    Summarize how quickly the algorithm converged to a stable best cost.

    Returns
    -------
    dict with keys:
        first_valid_iteration : int (first iteration that found any path)
        stable_iteration      : int (first iteration where cost stopped improving)
        final_cost            : float
    """
    first_valid = None
    stable = None
    prev_cost = None

    for i, cost in enumerate(history):
        if cost is not None and first_valid is None:
            first_valid = i
        if cost is not None and cost != prev_cost:
            stable = i
        prev_cost = cost

    final_cost = history[-1] if history else None

    return {
        "first_valid_iteration": first_valid,
        "stable_iteration": stable,
        "final_cost": final_cost,
    }


def print_convergence(history: list):
    """Print a simple ASCII convergence chart."""
    valid = [(i, c) for i, c in enumerate(history) if c is not None]
    if not valid:
        print("  No valid paths found during any iteration.")
        return

    costs = [c for _, c in valid]
    min_cost = min(costs)
    max_cost = max(costs)
    bar_width = 30

    print("\n  Convergence over iterations (sample):")
    print("  " + "-" * (bar_width + 20))

    # Show up to 20 evenly-spaced sample iterations
    step = max(1, len(valid) // 20)
    for i, cost in valid[::step]:
        if max_cost > min_cost:
            filled = int((cost - min_cost) / (max_cost - min_cost) * bar_width)
        else:
            filled = bar_width
        bar = "█" * filled + "░" * (bar_width - filled)
        print(f"  Iter {i:4d} | {bar} | {cost:.2f}")

    print("  " + "-" * (bar_width + 20))
    print(f"  Best cost reached: {min_cost:.4f}\n")
