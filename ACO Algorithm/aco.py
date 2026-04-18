"""
Ant Colony Optimization (ACO) for Shortest Path Finding
--------------------------------------------------------
Each ant independently explores the graph, depositing pheromones
on the edges it visits. The shorter paths will always gains the most 
pheromones over iterations, guiding future ants toward better routes.
"""

import random
import math


class AntColonyOptimization:
    """
    Ant Colony Optimization algorithm for finding shortest paths in a graph.
    Parameters
    ----------
    graph : dict Adjacency list: { node: [(neighbor, weight), ...], ... }
    num_ants : int The number of ants sent out each iteration.
    num_iterations : int How many iterations to run.
    ph_influence : float Pheromone influence weight. Higher = ants follow pheromones more strongly.
    dist_influence : float Distance influence weight. Higher = ants prefer shorter edges.
    evaporate_rate : float Fraction of pheromone that evaporates each iteration (0.0 to 1.0).
    initial_ph : float Starting pheromone level on all edges.
    """

    def __init__(
        self,
        graph: dict,
        num_ants: int = 20,
        num_iterations: int = 100,
        ph_influence: float = 1.0,
        dist_influence: float = 2.0,
        evaporate_rate: float = 0.5,
        initial_ph: float = 1.0,
    ):
        self.graph = graph
        self.num_ants = num_ants
        self.num_iterations = num_iterations
        self.ph_influence = ph_influence
        self.dist_influence = dist_influence
        self.evaporate_rate = evaporate_rate
        self.initial_ph = initial_ph

        self.nodes = list(graph.keys())
        self.pheromones = self._initialize_pheromones()


    #How it sets up
    def _initialize_pheromones(self) -> dict:
        """Create a pheromone matrix keyed by (node_a, node_b)."""
        pheromones = {}
        for node, neighbors in self.graph.items():
            for neighbor, _ in neighbors:
                pheromones[(node, neighbor)] = self.initial_ph
                pheromones[(neighbor, node)] = self.initial_ph
        return pheromones

    #How it decides to move
    def _get_edge_weight(self, node_a, node_b) -> float:
        """Return the weight of a direct edge between two nodes."""
        for neighbor, weight in self.graph.get(node_a, []):
            if neighbor == node_b:
                return weight
        return math.inf

    def _choose_next_node(self, current: str, visited: set, destination: str) -> str | None:
        
        #This determines the probability of picking the next node an ant will move to.
        #Returns None if the ant is completely stuck (no unvisited neighbors).
        
        neighbors = [
            (neighbor, weight)
            for neighbor, weight in self.graph.get(current, [])
            if neighbor not in visited
        ]

        # Always move to destination if it is directly reachable
        for neighbor, weight in neighbors:
            if neighbor == destination:
                return destination

        if not neighbors:
            return None

        attractiveness = []
        for neighbor, weight in neighbors:
            pheromone = self.pheromones.get((current, neighbor), self.initial_ph)
            heuristic = 1.0 / weight if weight > 0 else 1e9
            attractiveness.append(
                (neighbor, (pheromone ** self.ph_influence) * (heuristic ** self.dist_influence))
            )

        total = sum(score for _, score in attractiveness)
        if total == 0:
            return random.choice([n for n, _ in attractiveness])

        # Roulette-wheel selection
        roll = random.uniform(0, total)
        cumulative = 0.0
        for neighbor, score in attractiveness:
            cumulative += score
            if roll <= cumulative:
                return neighbor

        return attractiveness[-1][0]

    def _run_ant(self, source: str, destination: str) -> tuple[list, float]:
        """
        Send a single ant from source to destination.

        Returns
        -------
        path : list of nodes (empty if destination was not reached)
        total_cost : float (inf if destination was not reached)
        """
        path = [source]
        visited = {source}
        total_cost = 0.0

        while path[-1] != destination:
            current = path[-1]
            next_node = self._choose_next_node(current, visited, destination)

            if next_node is None:
                return [], math.inf  # Ant got stuck

            edge_weight = self._get_edge_weight(current, next_node)
            total_cost += edge_weight
            path.append(next_node)
            visited.add(next_node)

            # Prevent runaway paths on very large graphs
            if len(path) > len(self.nodes) + 1:
                return [], math.inf

        return path, total_cost

    #How the ph(eromones) are updated
    def _evaporate_pheromones(self):
        #Reduce all pheromone levels by the evaporation rate.
        for edge in self.pheromones:
            self.pheromones[edge] *= (1.0 - self.evaporate_rate)
            self.pheromones[edge] = max(self.pheromones[edge], 1e-10)  # floor

    def _deposit_pheromones(self, path: list, total_cost: float):
        """
        Deposit pheromones along a completed path.
        Shorter paths deposit more pheromone (1 / cost).
        """
        if not path or total_cost == 0 or total_cost == math.inf:
            return

        deposit = 1.0 / total_cost
        for i in range(len(path) - 1):
            edge = (path[i], path[i + 1])
            reverse = (path[i + 1], path[i])
            self.pheromones[edge] = self.pheromones.get(edge, 0) + deposit
            self.pheromones[reverse] = self.pheromones.get(reverse, 0) + deposit

    # ------------------------------------------------------------------
    # Main solve loop
    # ------------------------------------------------------------------

    def solve(self, source: str, destination: str) -> dict:
        """
        Run the ACO algorithm to find the shortest path from source to destination.

        Returns
        -------
        dict with keys:
            best_path   : list of node names
            best_cost   : float
            iteration   : int (iteration where best was found)
            history     : list of best costs per iteration
        """
        if source not in self.graph:
            raise ValueError(f"Source node '{source}' not found in graph.")
        if destination not in self.graph:
            raise ValueError(f"Destination node '{destination}' not found in graph.")

        best_path = []
        best_cost = math.inf
        best_iteration = -1
        history = []

        for iteration in range(self.num_iterations):
            iteration_best_path = []
            iteration_best_cost = math.inf

            for _ in range(self.num_ants):
                path, cost = self._run_ant(source, destination)
                if cost < iteration_best_cost:
                    iteration_best_cost = cost
                    iteration_best_path = path

                # Deposit pheromones immediately (ant-by-ant update)
                self._deposit_pheromones(path, cost)

            # Evaporate after all ants have moved
            self._evaporate_pheromones()

            if iteration_best_cost < best_cost:
                best_cost = iteration_best_cost
                best_path = iteration_best_path
                best_iteration = iteration

            history.append(best_cost if best_cost != math.inf else None)

        return {
            "best_path": best_path,
            "best_cost": best_cost,
            "iteration": best_iteration,
            "history": history,
        }
