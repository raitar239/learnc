import subprocess
import matplotlib.pyplot as plt
import re

def get_execution_time(n, threads):
    try:
        result = subprocess.run(['./build/4task9', str(n), str(threads)], capture_output=True, text=True)
        match = re.search(r"Execution time: ([\d.]+)", result.stdout)
        if match:
            return float(match.group(1))
    except Exception as e:
        print(f"Error running program: {e}")

matrix_sizes = [250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500]
thread_counts = [1, 2, 4, 8, 16, 32] 

plt.figure(figsize=(12, 7))

for t in thread_counts:
    times = []
    print(f"Testing {t} threads...")
    for n in matrix_sizes:
        exec_time = get_execution_time(n, t)
        times.append(exec_time)
        print(f"    Size {n}: {exec_time:.4f}s")
    plt.plot(matrix_sizes, times, marker='o', label=f'{t} threads')

plt.title('Matrix multiplication time as a function of size and threads')
plt.xlabel('Matrix size (N*N)')
plt.ylabel('Execution time (seconds)')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()
plt.tight_layout()

plt.savefig("4task9.png")
