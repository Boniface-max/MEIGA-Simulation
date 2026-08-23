import json
import matplotlib.pyplot as plt
import numpy as np

# 1. Load the JSON detector configuration
with open("output.json", "r") as f:
    config = json.load(f)

print("Loaded Detector Configuration:")
print(json.dumps(config, indent=2))

# 2. Load the ASCII simulation hit matrix
# vertical_muon.txt contains space-separated values for each event
data = np.loadtxt("vertical_muon.txt")

total_events = data.shape[0]
num_columns = data.shape[1]

print(f"\n--- Simulation Data Summary ---")
print(f"Total simulated events: {total_events}")
print(f"Total data columns per event: {num_columns}")

# 3. Separate event metadata and hit channels
# If the first column is the event ID/counter, separate it from channel responses
event_ids = data[:, 0]
hit_matrix = data[:, 1:]

# 4. Calculate Key Metrics

# Total hits recorded on each channel/panel across all events
channel_hits = np.sum(hit_matrix > 0, axis=0)

# Events where ANY panel recorded a hit (at least 1)
any_panel_hits = np.sum(np.any(hit_matrix[:, :3] > 0, axis=1))

# Events where ALL 3 panels recorded a hit simultaneously (3-panel coincidence)
all_3_panels_hits = np.sum(np.all(hit_matrix[:, :3] > 0, axis=1))

# Detection efficiencies
triggered_events = any_panel_hits
efficiency = (triggered_events / total_events) * 100
coincidence_efficiency = (all_3_panels_hits / total_events) * 100

print(f"\n================ DETECTOR MUON COUNTS ================")
print(f"Top Hodoscope Hits (ID 0)    : {channel_hits[0]}")
print(f"Middle eCounter Hits (ID 1)  : {channel_hits[1]}")
print(f"Bottom Hodoscope Hits (ID 2) : {channel_hits[2]}")
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
    channel_hits[:3],
    color=["navy", "teal", "crimson"],
)
plt.ylabel("Hit Count")
plt.title("Muon Hit Occupancy per Detector Panel")
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()
plt.savefig("hit_occupancy.png")
print("\nPlot saved successfully as 'hit_occupancy.png'.")
