import pandas as pd
import matplotlib.pyplot as plt

def plot_results():
    try:
        df = pd.read_csv("results-200-cities.csv")
    except FileNotFoundError:
        print("Error: results.csv not found. Run benchmark.py first!")
        return

    threads = df["Threads"].values
    times = df["AvgTime"].values
    
    base_time = times[0]
    speedup = [base_time / t for t in times]
    ideal_speedup = threads # Ideal is perfectly linear scaling

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    ax1.plot(threads, times, marker='o', linestyle='-', color='b', linewidth=2)
    ax1.set_title('Execution Time vs Threads', fontsize=14)
    ax1.set_xlabel('Number of Threads', fontsize=12)
    ax1.set_ylabel('Average Time (Seconds)', fontsize=12)
    ax1.set_xticks(threads)
    ax1.grid(True, linestyle='--', alpha=0.7)

    ax2.plot(threads, ideal_speedup, linestyle='--', color='gray', label='Ideal Scaling')
    ax2.plot(threads, speedup, marker='s', linestyle='-', color='r', linewidth=2, label='Actual Speedup')
    ax2.set_title('Parallel Speedup (Amdahl\'s Law)', fontsize=14)
    ax2.set_xlabel('Number of Threads', fontsize=12)
    ax2.set_ylabel('Speedup Factor', fontsize=12)
    ax2.set_xticks(threads)
    ax2.legend()
    ax2.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig("scaling_results.png", dpi=300)
    print("Graph saved as 'scaling_results.png'")
    plt.show()

if __name__ == "__main__":
    plot_results()