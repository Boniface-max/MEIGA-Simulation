import json
import matplotlib.pyplot as plt
import numpy as np

# 1. Load JSON detector configuration
with open("output.json", "r") as f:
    config = json.load(f)

# 2. Load ASCII simulation matrix
data = np.loadtxt("vertical_muon.txt")

total_events = data.shape[0]

# 3. Column Mapping matching eHodoscope & eCounter structures
# Column 0: Event ID
# Columns 1 & 2: Top eHodoscope channels (ID 0)
# Column 3: Middle eCounter channel (ID 1)
# Columns 4 to 11: Bottom eHodoscope channels (ID 2)

top_eHodoscope = np.any(data[:, 1:3] > 0, axis=1)
middle_eCounter = data[:, 3] > 0
bottom_eHodoscope = np.any(data[:, 4:] > 0, axis=1)

# 4. Individual Panel Counts
top_hits = np.sum(top_eHodoscope)
middle_hits = np.sum(middle_eCounter)
bottom_hits = np.sum(bottom_eHodoscope)

# 5. Coincidence Logic (Top eHodoscope + Middle eCounter + Bottom eHodoscope)
coincidence_3_panel = top_eHodoscope & middle_eCounter & bottom_eHodoscope
any_panel_trigger = top_eHodoscope | middle_eCounter | bottom_eHodoscope

all_3_panels_hits = np.sum(coincidence_3_panel)
any_panel_hits = np.sum(any_panel_trigger)

# 6. Efficiency Calculations
top_eff = (top_hits / total_events) * 100
middle_eff = (middle_hits / total_events) * 100
bottom_eff = (bottom_hits / total_events) * 100
coincidence_3_panel_eff = (all_3_panels_hits / total_events) * 100

# 7. Print Terminal Output with exact detector names
print(f"\n================ DETECTOR MUON COUNTS ================")
print(f"Top eHodoscope Hits (ID 0)    : {top_hits}")
print(f"Middle eCounter Hits (ID 1)   : {middle_hits}")
print(f"Bottom eHodoscope Hits (ID 2) : {bottom_hits}")
print(f"------------------------------------------------------")
print(f"Muons hitting ANY panel       : {any_panel_hits}")
print(f"Muons hitting ALL 3 panels    : {all_3_panels_hits}")
print(f"======================================================")
print(f"Top eHodoscope Efficiency     : {top_eff:.2f}%")
print(f"Middle eCounter Efficiency    : {middle_eff:.2f}%")
print(f"Bottom eHodoscope Efficiency  : {bottom_eff:.2f}%")
print(f"3-Panel Coincidence Efficiency: {coincidence_3_panel_eff:.2f}%")

# 8. Plot Occupancy with eHodoscope labels
plt.figure(figsize=(8, 5))
plt.bar(
    ["Top (eHodoscope)", "Middle (eCounter)", "Bottom (eHodoscope)"],
    [top_hits, middle_hits, bottom_hits],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy Across eHodoscope & eCounter Panels")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
print("\nPlot saved successfully as 'hit_occupancy.png'.")
