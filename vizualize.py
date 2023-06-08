import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys
import json

if len(sys.argv) > 1:
    filename = sys.argv[1]
else:
    print("Please provide a filename")
    sys.exit(1)

with open(filename) as f:
    data = json.load(f)

# Extract the benchmark names and times from the data
benchmarsks = [bench["name"] for bench in data["benchmarks"]]
times = [bench["cpu_time"] for bench in data["benchmarks"]]

plt.figure(figsize=(10, 6))
plt.plot(benchmarsks, times, "o-")
plt.xlabel("Benchmark")
plt.ylabel("Time (ns)")
plt.title("Benchmark results")
plt.xticks(rotation=90)

# calculate the median
median = np.median(times)

# plot the median
plt.axhline(median, color="red", linestyle="--", label="Median")
plt.text(0, median, f"{median:.2f}", va="center", ha="left", bbox=dict(facecolor="w", alpha=0.5))

plt.savefig('plot.png')


