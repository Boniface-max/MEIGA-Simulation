from SimDataReader import *

outfile = './output.json'
simData = SimDataReader(outfile)

# Extract detector objects directly from simData
top_data = simData.GetDetectorSimData(det_id=0)      # Top Hodoscope
counter_data = simData.GetDetectorSimData(det_id=1)  # Middle eCounter
bottom_data = simData.GetDetectorSimData(det_id=2)   # Bottom Hodoscope

# Get arrays of deposited energy per event
energies_top = top_data.get_deposited_energy() if top_data else []
energies_counter = counter_data.get_deposited_energy() if counter_data else []
energies_bottom = bottom_data.get_deposited_energy() if bottom_data else []

total_events = max(len(energies_top), len(energies_counter), len(energies_bottom))
print(f"Total simulated events: {total_events}\n")

top_hits = 0
counter_hits = 0
bottom_hits = 0
any_panel_hits = 0
three_fold_hits = 0

# Loop through event indices
for i in range(total_events):
    e_top = energies_top[i] if i < len(energies_top) else 0.0
    e_counter = energies_counter[i] if i < len(energies_counter) else 0.0
    e_bottom = energies_bottom[i] if i < len(energies_bottom) else 0.0
    
    hit_top = e_top > 0.0
    hit_counter = e_counter > 0.0
    hit_bottom = e_bottom > 0.0
    
    if hit_top: 
        top_hits += 1
    if hit_counter: 
        counter_hits += 1
    if hit_bottom: 
        bottom_hits += 1
    
    # Hit condition across panels
    if hit_top or hit_counter or hit_bottom:
        any_panel_hits += 1
        
    # 3-Fold Coincidence (Top + Middle Counter + Bottom)
    if hit_top and hit_counter and hit_bottom:
        three_fold_hits += 1

print("================ DETECTOR MUON COUNTS ================")
print(f"Top Hodoscope Hits (ID 0)     : {top_hits}")
print(f"Middle eCounter Hits (ID 1)   : {counter_hits}")
print(f"Bottom Hodoscope Hits (ID 2)  : {bottom_hits}")
print("------------------------------------------------------")
print(f"Muons hitting ANY panel       : {any_panel_hits}")
print(f"Muons hitting ALL 3 panels    : {three_fold_hits}")
print("======================================================")
