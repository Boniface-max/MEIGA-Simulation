from SimDataReader import *

# Load uncompressed simulation data
outfile = './output.json'
simData = SimDataReader(outfile)

# Primary injected particle flux
input_flux = simData.get_input_flux()

# Map detector IDs according to DetectorList.xml specifications
detectors = {
    0: {"type": "eHodoscope", "name": "Top Hodoscope", "z": -1.0},
    1: {"type": "eCounter",   "name": "Middle Counter", "z": -2.0},
    2: {"type": "eHodoscope", "name": "Bottom Hodoscope", "z": -3.0}
}

# Iterate through panels to extract energy and photoelectron time traces
for det_id, info in detectors.items():
    print(f"\n--- [ID {det_id}] {info['name']} ({info['type']}) at z = {info['z']} m ---")
    
    detSimData = simData.GetDetectorSimData(det_id=det_id)
    
    # Deposited energy in detector volume
    deposited_energy = detSimData.get_deposited_energy()
    print(f"Deposited Energy: {deposited_energy}")
    
    # Readout device timing distributions (SiPM/PMT index 0)
    odSimData = detSimData.GetOptDeviceSimData(od_id=0)
    pe_time_dist = odSimData.get_pe_time_distribution()
    print(f"PE Time Traces: {pe_time_dist}")
