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

# 3. Correct Column Mapping
# Column 0: Event ID
# Column 3 (data[:, 3]): Muon Counter (Middle eCounter)
# Columns 1, 2, 4+: Hodoscope Channels
counter_hits = data[:, 3] > 0

# Separate hodoscope matrix by removing Event ID (0) and Muon Counter (3)
hodoscope_hits = np.delete(data, [0, 3], axis=1)

# Extract Top and Bottom Hodoscope Panels
# Adjust slice indices [:, 0] and [:, 1] if your hodoscope channels use higher column indices
top_hodoscope = hodoscope_hits[:, 0] > 0
bottom_hodoscope = hodoscope_hits[:, 1] > 0

# 4. Calculate Individual Hit Totals
top_count = np.sum(top_hodoscope)
middle_counter_count = np.sum(counter_hits)
bottom_count = np.sum(bottom_hodoscope)

# 5. Coincidence and Trigger Logic Across ALL Panels
coincidence_3_panel = top_hodoscope & counter_hits & bottom_hodoscope
any_panel_trigger = top_hodoscope | counter_hits | bottom_hodoscope

all_3_panels_hits = np.sum(coincidence_3_panel)
any_panel_hits = np.sum(any_panel_trigger)

# Efficiency Calculations
counter_eff = (middle_counter_count / total_events) * 100
hodoscope_eff = (np.sum(np.any(hodoscope_hits > 0, axis=1)) / total_events) * 100
coincidence_3_panel_eff = (all_3_panels_hits / total_events) * 100

print(f"\n================ DETECTOR MUON COUNTS ================")
print(f"Top Hodoscope Hits (ID 0)    : {top_count}")
print(f"Middle eCounter Hits (ID 1)  : {middle_counter_count}")
print(f"Bottom Hodoscope Hits (ID 2) : {bottom_count}")
print(f"------------------------------------------------------")
print(f"Muons hitting ANY panel      : {any_panel_hits}")
print(f"Muons hitting ALL 3 panels   : {all_3_panels_hits}")
print(f"======================================================")
print(f"Muon Counter Efficiency      : {counter_eff:.2f}%")
print(f"Hodoscope Tracking Efficiency: {hodoscope_eff:.2f}%")
print(f"3-Panel Coincidence Efficiency: {coincidence_3_panel_eff:.2f}%")

# 6. Plot Occupancy Across All Panels
plt.figure(figsize=(8, 5))
plt.bar(
    ["Top Hodoscope", "Middle eCounter", "Bottom Hodoscope"],
    [top_count, middle_counter_count, bottom_count],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy Across All Detector Panels")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
print("\nPlot saved successfully as 'hit_occupancy.png'.")
