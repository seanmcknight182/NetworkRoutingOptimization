import os
import re
import csv
import matplotlib.pyplot as plt

ALGO_DIRS = [
    "ACO_Algorithm",
    "A_Star_Algorithm",
    "Algorithm1Dijkstra",
    "Bellman_Ford_Algorithm"
]

# Example filenames: results_50.csv, results_100.csv
FILE_PATTERN = r"results_(\d+)\.csv"


def parse_metrics(filepath):
    with open(filepath, "r") as f:
        reader = csv.DictReader(f)
        row = next(reader)  # Only one row expected

        return {
            "time": int(row["time_ms"]),
            "ops": int(row["basic_op_count"]),
            "weight": int(row["weight"])
        }


def collect_data():
    data = {}

    for algo in ALGO_DIRS:
        algo_data = []
        if not os.path.exists(algo):
            continue

        for file in os.listdir(algo):
            match = re.match(FILE_PATTERN, file)
            if match:
                num_points = int(match.group(1))
                filepath = os.path.join(algo, file)

                metrics = parse_metrics(filepath)
                algo_data.append((num_points, metrics))

        # Sort by number of points
        algo_data.sort(key=lambda x: x[0])
        data[algo] = algo_data

    return data


def plot_metric(data, metric_key, ylabel, filename):
    plt.figure()

    for algo, values in data.items():
        x = [v[0] for v in values]
        y = [v[1][metric_key] for v in values]

        plt.plot(x, y, marker='o', label=algo)

    plt.xlabel("Number of Points")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs Number of Points")
    plt.legend()
    plt.grid(True)

    plt.savefig(filename)
    plt.close()


def main():
    data = collect_data()

    if not data:
        print("No data found.")
        return

    plot_metric(data, "time", "Time (ms)", "time_plot.png")
    plot_metric(data, "ops", "Basic Operation Count", "ops_plot.png")
    plot_metric(data, "weight", "Weight", "weight_plot.png")

    print("Graphs saved: time_plot.png, ops_plot.png, weight_plot.png")


if __name__ == "__main__":
    main()
