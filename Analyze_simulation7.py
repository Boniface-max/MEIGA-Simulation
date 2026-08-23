import json
import matplotlib.pyplot as plt
import numpy as np

# 1. Load the JSON detector configuration
with open("output.json", "r") as f:
    config = json.load(f)

print("Loaded Detector Configuration:")
print(json.dumps(config, indent=2))

# 2. Load the ASCII simulation hit matrix
data = np.loadtxt("vertical_muon.txt")

total_events = data.shape[0]
num_columns = data.shape[1]

print(f"\n--- Simulation Data Summary ---")
print(f"Total simulated events: {total_events}")
print(f"Total data columns per event: {num_columns}")

# 3. Separate event metadata and hit channels
event_ids = data[:, 0]
hit_matrix = data[:, 1:]

# Diagnostics: Print total hits recorded in every column to locate panel channels
print("\n--- Column Diagnostics (Hits > 0) ---")
col_hits = np.sum(hit_matrix > 0, axis=0)
for idx, count in enumerate(col_hits):
    print(f"  Column index {idx} (Data col {idx+1}): {count} hits")

# 4. Calculate Key Metrics
# If panel channels are located at specific column indices, update the slice/indices below:
top_panel = hit_matrix[:, 0] > 0
middle_panel = hit_matrix[:, 1] > 0
bottom_panel = hit_matrix[:, 2] > 0

# Count individual hits
top_hits = np.sum(top_panel)
middle_hits = np.sum(middle_panel)
bottom_hits = np.sum(bottom_panel)

# Coincidence and trigger logic
coincidence_mask = top_panel & middle_panel & bottom_panel
any_mask = top_panel | middle_panel | bottom_panel

all_3_panels_hits = np.sum(coincidence_mask)
any_panel_hits = np.sum(any_mask)

# Efficiency calculations
triggered_events = any_panel_hits
efficiency = (triggered_events / total_events) * 100
coincidence_efficiency = (all_3_panels_hits / total_events) * 100

print(f"\n================ DETECTOR MUON COUNTS ================")
print(f"Top Hodoscope Hits (ID 0)    : {top_hits}")
print(f"Middle eCounter Hits (ID 1)  : {middle_hits}")
print(f"Bottom Hodoscope Hits (ID 2) : {bottom_hits}")
print(f"------------------------------------------------------")
print(f"Muons hitting ANY panel      : {any_panel_hits}")
print(f"Muons hitting ALL 3 panels   : {all_3_panels_hits}")
print(f"======================================================")
print(f"Triggered Events             : {triggered_events} / {total_events}")
print(f"Any-Panel Detection Efficiency : {efficiency:.2f}%")
print(f"3-Panel Coincidence Efficiency : {coincidence_efficiency:.2f}%")

# 5. Plot Hit Occupancy
plt.figure(figsize=(8, 5))
plt.bar(
    ["Top (ID 0)", "Middle (ID 1)", "Bottom (ID 2)"],
    [top_hits, middle_hits, bottom_hits],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy per Detector Panel")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
print("\nPlot saved successfully as 'hit_occupancy.png'.")
