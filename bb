import numpy as np
import scipy.stats as stats
import os

# =====================================================================
# 1. DATA PARSING & INGESTION
# =====================================================================
def load_meiga_simulation_data(filepath, thresh_1cm=0.3, thresh_5cm=1.0):
    """
    Parses tracking output data from the MEIGA G4HodoscopeSimulator.
    Modify column indices below based on your specific ASCII/CSV log format.
    """
    if not os.path.exists(filepath):
        print(f"⚠️ Data file not found: {filepath}. Using Moyal fallback distribution for debugging.")
        return generate_fallback_data()

    print(f"📦 Loading data from: {filepath}...")
    
    # Assuming standard structured columns: 
    # [eventID, p1_edep, p2_edep, p3_edep, incident_angle]
    data = np.genfromtxt(filepath, delimiter=',', skip_header=1)
    
    # Extract values from data columns
    E_p1_raw = data[:, 1]
    E_p2_raw = data[:, 2]
    E_p3_raw = data[:, 3]
    angles = data[:, 4] if data.shape[1] > 4 else np.zeros(len(data))
    
    N_events = len(data)
    
    # Apply detection logic thresholds (Energy threshold check)
    hit_p1 = E_p1_raw > thresh_1cm
    hit_p2 = E_p2_raw > thresh_5cm
    hit_p3 = E_p3_raw > thresh_1cm
    
    # Enforce zeros for channels below threshold readout noise
    E_p1 = np.where(hit_p1, E_p1_raw, 0.0)
    E_p2 = np.where(hit_p2, E_p2_raw, 0.0)
    E_p3 = np.where(hit_p3, E_p3_raw, 0.0)
    
    return N_events, hit_p1, hit_p2, hit_p3, E_p1, E_p2, E_p3, angles

def generate_fallback_data():
    """Fallback debugger utilizing fixed SciPy Moyal distributions"""
    N_events = 10000
    E_1cm_ideal = stats.moyal.rvs(loc=2.0, scale=0.3, size=N_events)
    E_5cm_ideal = stats.moyal.rvs(loc=10.0, scale=1.2, size=N_events)
    
    hit_p1 = (E_1cm_ideal > 0.3) & (np.random.rand(N_events) < 0.98)
    hit_p2 = (E_5cm_ideal > 1.0) & (np.random.rand(N_events) < 0.99)
    hit_p3 = (E_1cm_ideal > 0.3) & (np.random.rand(N_events) < 0.97)
    
    return (N_events, hit_p1, hit_p2, hit_p3, 
            np.where(hit_p1, E_1cm_ideal, 0.0), 
            np.where(hit_p2, E_5cm_ideal, 0.0), 
            np.where(hit_p3, E_1cm_ideal, 0.0), 
            np.zeros(N_events))

# =====================================================================
# 2. PERFORMANCE CRITERIA ANALYSIS ENGINE
# =====================================================================
# Specify your text/CSV output filename generated from G4HodoscopeSimulator here:
DATA_FILE = "detector_hits.csv" 

N_events, hit_p1, hit_p2, hit_p3, E_p1, E_p2, E_p3, angles = load_meiga_simulation_data(DATA_FILE)

print("\n=== MEIGA DETECTOR PERFORMANCE ANALYSIS ===")
print(f"Total Logged Muon Events: {N_events}\n")

# --- A. Detection Efficiency ---
eff_p1 = np.sum(hit_p1) / N_events
eff_p2 = np.sum(hit_p2) / N_events
eff_p3 = np.sum(hit_p3) / N_events

print("--- 1. Panel Single Efficiencies ---")
print(f"Panel 1 (Top, 1 cm):    {eff_p1*100:.2f}%")
print(f"Panel 2 (Middle, 5 cm): {eff_p2*100:.2f}%")
print(f"Panel 3 (Bottom, 1 cm): {eff_p3*100:.2f}%\n")

# --- B. Coincidence Logic ---
coinc_1_2 = np.sum(hit_p1 & hit_p2)
coinc_2_3 = np.sum(hit_p2 & hit_p3)
coinc_1_3 = np.sum(hit_p1 & hit_p3)
coinc_triple = np.sum(hit_p1 & hit_p2 & hit_p3)

print("--- 2. Coincidence Summary Matrix ---")
print(f"Double Coincidence (P1 & P2): {coinc_1_2} events ({coinc_1_2/N_events*100:.2f}%)")
print(f"Double Coincidence (P2 & P3): {coinc_2_3} events ({coinc_2_3/N_events*100:.2f}%)")
print(f"Double Coincidence (P1 & P3): {coinc_1_3} events ({coinc_1_3/N_events*100:.2f}%)")
print(f"Triple Coincidence (P1 & P2 & P3): {coinc_triple} events ({coinc_triple/N_events*100:.2f}%)\n")

# --- C. Energy Deposition Profile Verification ---
mean_E_p1 = np.mean(E_p1[E_p1 > 0]) if np.any(E_p1 > 0) else 0
mean_E_p2 = np.mean(E_p2[E_p2 > 0]) if np.any(E_p2 > 0) else 0
ratio = mean_E_p2 / mean_E_p1 if mean_E_p1 > 0 else 0

print("--- 3. Extracted Energy Deposition Profiles ---")
print(f"P1 (1 cm) Mean Energy: {mean_E_p1:.3f} MeV")
print(f"P2 (5 cm) Mean Energy: {mean_E_p2:.3f} MeV")
print(f"Thickness Scaling Empirical Ratio: {ratio:.2f}x (Expected Linear Trend: ~5.00x)\n")

# --- D. Angular Dependent Efficiencies ---
unique_angles = np.unique(angles)
if len(unique_angles) > 1:
    print("--- 4. Angular Transmission Response ---")
    for ang in unique_angles:
        mask = (angles == ang)
        ang_events = np.sum(mask)
        ang_coinc = np.sum(hit_p1[mask] & hit_p2[mask] & hit_p3[mask])
        ang_eff = ang_coinc / ang_events if ang_events > 0 else 0
        print(f"Incident Tracker Angle: {ang:5.1f}° | Triple Coincidence Eff: {ang_eff*100:.2f}%")
