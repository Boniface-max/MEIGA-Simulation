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

# Bottom Panel: Check if any bottom hodoscope channels (Cols 4-12) fired
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
