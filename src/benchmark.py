import subprocess
import re
import csv
import time

EXECUTABLE = "./tsp_solver"
INPUT_FILE = "../run/input.dat"
POPULATION = 2000
GENERATIONS = 2000
TRIALS = 5
THREAD_COUNTS = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

time_pattern = re.compile(r"EXECUTION_TIME:\s+([0-9.]+)\s+seconds")
distance_pattern = re.compile(r"Ultimate Distance:\s+([0-9]+)")

def run_benchmark():
    results = []
    
    print(f"Starting Benchmark: {GENERATIONS} generations, {POPULATION} pop size")
    print("-" * 50)

    for threads in THREAD_COUNTS:
        total_time = 0.0
        best_dist = float('inf')
        
        print(f"Testing {threads} Threads...")
        
        for _ in range(TRIALS):
            cmd = [
                EXECUTABLE, 
                "-t", str(threads), 
                "-p", str(POPULATION), 
                "-g", str(GENERATIONS),
                "-i", INPUT_FILE
            ]
            
            process = subprocess.run(cmd, capture_output=True, text=True)
            output = process.stdout
            
            time_match = time_pattern.search(output)
            dist_match = distance_pattern.search(output)
            
            if time_match and dist_match:
                t = float(time_match.group(1))
                d = int(dist_match.group(1))
                total_time += t
                best_dist = min(best_dist, d)
            else:
                print("Error: Could not parse C++ output. Did it crash?")
                print(output)
                return

        avg_time = total_time / TRIALS
        print(f"  -> Avg Time: {avg_time:.4f}s | Best Dist: {best_dist}")
        
        results.append({
            "Threads": threads,
            "AvgTime": avg_time,
            "BestDistance": best_dist
        })

    csv_file = "results.csv"
    with open(csv_file, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=["Threads", "AvgTime", "BestDistance"])
        writer.writeheader()
        writer.writerows(results)
        
    print("-" * 50)
    print(f"Benchmark complete! Data saved to {csv_file}")

if __name__ == "__main__":
    run_benchmark()