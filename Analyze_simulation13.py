import numpy as np

# 1. Extract the counter hits (Column 0 as per your script)
counter_hits = hit_matrix[:, 0]

# 2. Extract the hodoscope hits (All remaining columns)
hodoscope_hits = hit_matrix[:, 1:]

# 3. Calculate Coincidence (Events with hits in BOTH the counter AND any hodoscope panel)
# np.any(..., axis=1) checks if there is a hit in at least one of the hodoscope columns per event
coincidence_mask = (counter_hits > 0) & np.any(hodoscope_hits > 0, axis=1)
coincidence_events = np.sum(coincidence_mask)

# 4. Calculate and print the coincidence efficiency
total_events = len(hit_matrix)
coincidence_efficiency = (coincidence_events / total_events) * 100

print(f"Muon Counter Efficiency: {(np.sum(counter_hits > 0) / total_events) * 100:.2f}%")
print(f"Hodoscope Coincidence Efficiency: {coincidence_efficiency:.2f}%")
