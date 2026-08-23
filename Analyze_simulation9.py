import json
import matplotlib.pyplot as plt
import numpy as np

# 1. Load JSON configuration
with open("output.json", "r") as f:
    config = json.load(f)

print("Loaded Detector Configuration:")
print(json.dumps(config, indent=2))

# 2. Load ASCII simulation matrix
data = np.loadtxt("vertical_muon.txt")

total_events = data.shape[0]
num_columns = data.shape[1]

print(f"\n--- Simulation Data Summary ---")
print(f"Total simulated events: {total_events}")
print(f"Total data columns per event: {num_columns}")

# 3. Dynamic Channel Mapping
# Event ID is Column 0
# Middle eCounter is at Column 3 (Index 3 in 0-based indexing)
middle_counter = data[:, 3] > 0

# Check non-zero hit channels across remaining columns
# Index 1 = Top Panel candidate, Index 2 = Bottom Panel candidate
top_hodoscope = data[:, 1] > 0
bottom_hodoscope = data[:, 2] > 0

# If Column 2 is duplicate counter data, fall back to next available channels
if np.array_equal(data[:, 2] > 0, middle_counter):
    bottom_hodoscope = data[:, 4] > 0

# 4. Individual Panel Counts
top_hits = np.sum(top_hodoscope)
middle_hits = np.sum(middle_counter)
bottom_hits = np.sum(bottom_hodoscope)

# 5. Coincidence Logic
coincidence_3_panel = top_hodoscope & middle_counter & bottom_hodoscope
any_panel_trigger = top_hodoscope | middle_counter | bottom_hodoscope

all_3_panels_hits = np.sum(coincidence_3_panel)
any_panel_hits = np.sum(any_panel_trigger)

# Efficiency Calculations
counter_eff = (middle_hits / total_events) * 100
top_eff = (top_hits / total_events) * 100
bottom_eff = (bottom_hits / total_events) * 100
coincidence_3_panel_eff = (all_3_panels_hits / total_events) * 100

# 6. Terminal Display
print(f"\n================ DETECTOR MUON COUNTS ================")
print(f"Top Hodoscope Hits (ID 0)    : {top_hits}")
print(f"Middle eCounter Hits (ID 1)  : {middle_hits}")
print(f"Bottom Hodoscope Hits (ID 2) : {bottom_hits}")
print(f"------------------------------------------------------")
print(f"Muons hitting ANY panel      : {any_panel_hits}")
print(f"Muons hitting ALL 3 panels   : {all_3_panels_hits}")
print(f"======================================================")
print(f"Top Hodoscope Efficiency     : {top_eff:.2f}%")
print(f"Middle eCounter Efficiency   : {counter_eff:.2f}%")
print(f"Bottom Hodoscope Efficiency  : {bottom_eff:.2f}%")
print(f"3-Panel Coincidence Efficiency: {coincidence_3_panel_eff:.2f}%")

# 7. Plot Occupancy Across All 3 Panels
plt.figure(figsize=(8, 5))
plt.bar(
    ["Top (eHodoscope)", "Middle (eCounter)", "Bottom (eHodoscope)"],
    [top_hits, middle_hits, bottom_hits],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy Across Detector Panels")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
print("\nPlot saved successfully as 'hit_occupancy.png'.")
