from SimDataReader import *

# Path to output file
outfile = './output.json.gz'

# Create SimDataReader
simData = SimDataReader(outfile)

# --------------------------------------------------
# Check events
# --------------------------------------------------

event_ids = simData.get_event_list()

print("\n======================================")
print("MEIGA OUTPUT DIAGNOSTIC")
print("======================================")

print("[INFO] Number of events =", len(event_ids))

# --------------------------------------------------
# Check InputFlux
# --------------------------------------------------

inputFlux = simData.get_input_flux()

print("\n[INFO] InputFlux:")
print(inputFlux)

print("\n[INFO] InputFlux shape =", inputFlux.shape)

# --------------------------------------------------
# Check DetectorList
# --------------------------------------------------

detectorList = simData.get_detector_list()

print("\n[INFO] DetectorList:")
print(detectorList)

detector_ids = detectorList.loc['ID']

print("\n[INFO] Detector IDs =", detector_ids)

# --------------------------------------------------
# Detector 0 - eHodoscope
# --------------------------------------------------

print("\n--------------------------------------")
print("Detector 0 - eHodoscope")
print("--------------------------------------")

det0 = simData.GetDetectorSimData(det_id=0)

energy0 = det0.get_deposited_energy()
binary0 = det0.get_binary_counter()

print("[INFO] Deposited energy:")
print(energy0)

print("[INFO] Binary counter:")
print(binary0)

# --------------------------------------------------
# Detector 1 - eCounter
# --------------------------------------------------

print("\n--------------------------------------")
print("Detector 1 - eCounter")
print("--------------------------------------")

det1 = simData.GetDetectorSimData(det_id=1)

energy1 = det1.get_deposited_energy()

print("[INFO] Deposited energy:")
print(energy1)

# --------------------------------------------------
# Detector 2 - eHodoscope
# --------------------------------------------------

print("\n--------------------------------------")
print("Detector 2 - eHodoscope")
print("--------------------------------------")

det2 = simData.GetDetectorSimData(det_id=2)

energy2 = det2.get_deposited_energy()
binary2 = det2.get_binary_counter()

print("[INFO] Deposited energy:")
print(energy2)

print("[INFO] Binary counter:")
print(binary2)

print("\n======================================")
print("END OF DIAGNOSTIC")
print("======================================")
