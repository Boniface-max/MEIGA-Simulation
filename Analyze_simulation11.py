import json
import matplotlib.pyplot as plt
import numpy as np

# 1. Load JSON detector configuration
with open("output.json", "r") as f:
    config = json.load(f)

# 2. Load ASCII simulation matrix
data = np.loadtxt("vertical_muon.txt")

total_events = data.shape[0]

# 3. Explicit Column Slicing for All 3 Detector Layers
# Top Panel: Check if any top hodoscope channels (Cols 1-2) fired
top_hodoscope = np.any(data[:, 1:3] > 0, axis=1)

# Middle Panel: Check eCounter channel (Col 3)
middle_counter = data[:, 3] > 0

# Bottom Panel: Check if any bottom hodoscope channels (Cols 4-11) fired
bottom_hodoscope = np.any(data[:, 4:] > 0, axis=1)

# 4. Count Hits
top_hits = np.sum(top_hodoscope)
middle_hits = np.sum(middle_counter)
bottom_hits = np.sum(bottom_hodoscope)

# 5. Coincidence Logic Across ALL 3 Panels
coincidence_3_panel = top_hodoscope & middle_counter & bottom_hodoscope
any_panel_trigger = top_hodoscope | middle_counter | bottom_hodoscope

all_3_panels_hits = np.sum(coincidence_3_panel)
any_panel_hits = np.sum(any_panel_trigger)

# 6. Efficiency Calculations
counter_eff = (middle_hits / total_events) * 100
top_eff = (top_hits / total_events) * 100
bottom_eff = (bottom_hits / total_events) * 100
coincidence_3_panel_eff = (all_3_panels_hits / total_events) * 100

# 7. Print Terminal Output
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

# 8. Save Hit Plot
plt.figure(figsize=(8, 5))
plt.bar(
    ["Top Hodoscope", "Middle eCounter", "Bottom Hodoscope"],
    [top_hits, middle_hits, bottom_hits],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy Across All 3 Panels")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
