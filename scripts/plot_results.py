import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# 1. Load Data
csv_file = 'benchmark_data.csv'
if not os.path.exists(csv_file):
    print(f"Error: {csv_file} not found. Please run the batch file first.")
    exit()

df = pd.read_csv(csv_file)

# CLEANUP: Remove invisible spaces from column names (The fix for your error)
df.columns = df.columns.str.strip()

# CLEANUP: Remove invisible spaces from Benchmark names
df['Benchmark'] = df['Benchmark'].str.strip()

# Configuration
plt.style.use('ggplot')
benchmarks = df['Benchmark'].unique()
threads = sorted(df[df['Threads'] > 1]['Threads'].unique())
all_threads = sorted(df['Threads'].unique())

# Helper to get time
def get_time(bench, t):
    row = df[(df['Benchmark'] == bench) & (df['Threads'] == t)]
    return row['Time'].values[0] if not row.empty else None

# ==========================================
# 1. SEQUENTIAL VS PARALLEL (Bar Chart)
# ==========================================
print("Generating sequential_vs_parallel.png...")
fig, ax = plt.subplots(figsize=(10, 6))
width = 0.2
x = np.arange(len(benchmarks))

# Plot Sequential
seq_times = [get_time(b, 1) for b in benchmarks]
ax.bar(x - width*1.5, seq_times, width, label='Sequential', color='black')

# Plot Parallel Sets
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
for i, t in enumerate(threads):
    par_times = [get_time(b, t) for b in benchmarks]
    # Handle missing data if any
    par_times = [pt if pt is not None else 0 for pt in par_times]
    ax.bar(x + (i - 0.5) * width, par_times, width, label=f'{t} Threads', color=colors[i % len(colors)])

ax.set_ylabel('Execution Time (s)')
ax.set_title('Sequential vs Parallel Execution')
ax.set_xticks(x)
ax.set_xticklabels(benchmarks)
ax.legend()
ax.set_yscale('log') # Log scale because Matrix is much slower than Array
plt.tight_layout()
plt.savefig('sequential_vs_parallel.png')
plt.close()

# ==========================================
# 2. EXECUTION TIME (Line Graph)
# ==========================================
print("Generating execution_time.png...")
plt.figure(figsize=(10, 6))

for b in benchmarks:
    times = []
    valid_threads = []
    for t in all_threads:
        val = get_time(b, t)
        if val is not None:
            times.append(val)
            valid_threads.append(t)
    plt.plot(valid_threads, times, marker='o', label=b, linewidth=2)

plt.xlabel('Number of Threads')
plt.ylabel('Time (seconds)')
plt.title('Execution Time vs Threads')
plt.legend()
plt.grid(True)
plt.yscale('log')
plt.savefig('execution_time.png')
plt.close()

# ==========================================
# 3. SPEEDUP (Line Graph)
# ==========================================
print("Generating speedup.png...")
plt.figure(figsize=(10, 6))

for b in benchmarks:
    base_time = get_time(b, 1)
    if base_time is None: continue
    
    speedups = []
    valid_threads = []
    for t in all_threads:
        val = get_time(b, t)
        if val is not None:
            speedups.append(base_time / val)
            valid_threads.append(t)
            
    plt.plot(valid_threads, speedups, marker='o', label=b, linewidth=2)

# Ideal Line
plt.plot([1, max(threads)], [1, max(threads)], 'k--', alpha=0.5, label='Ideal Linear')

plt.xlabel('Number of Threads')
plt.ylabel('Speedup Factor')
plt.title('Speedup vs Threads')
plt.legend()
plt.grid(True)
plt.savefig('speedup.png')
plt.close()

# ==========================================
# 4. EFFICIENCY (Line Graph)
# ==========================================
print("Generating efficiency.png...")
plt.figure(figsize=(10, 6))

for b in benchmarks:
    base_time = get_time(b, 1)
    if base_time is None: continue

    efficiencies = []
    valid_threads = []
    for t in all_threads:
        val = get_time(b, t)
        if val is not None:
            # Efficiency = Speedup / Threads
            speedup = base_time / val
            efficiency = speedup / t
            efficiencies.append(efficiency)
            valid_threads.append(t)

    plt.plot(valid_threads, efficiencies, marker='o', label=b, linewidth=2)

plt.axhline(y=1.0, color='k', linestyle='--', alpha=0.5, label='Ideal Efficiency')
plt.xlabel('Number of Threads')
plt.ylabel('Efficiency (Speedup / Threads)')
plt.title('Parallel Efficiency vs Threads')
plt.legend()
plt.grid(True)
plt.ylim(0, 1.2) # Efficiency usually usually 0 to 1, sometimes >1 due to cache
plt.savefig('efficiency.png')
plt.close()

print("Success! All 4 plots generated.")