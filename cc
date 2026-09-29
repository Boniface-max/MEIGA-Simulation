import numpy as np
import scipy.stats as stats

# ==========================================
# 1. SIMULATED DATA GENERATION (for testing)
# ==========================================
# Let's generate a mock dataset of 10,000 muons to demonstrate the analysis.
# In production, replace this section by loading your actual simulation output file.
np.random.seed(42)
N_events = 10000

# Mean energy deposition for Minimum Ionizing Particles (MIPs) ~ 2 MeV/cm
# Landau distribution parameters: loc ~ mean peak, scale ~ fluctuations
E_1cm_ideal = stats.landau.rvs(loc=2.0, scale=0.3, size=N_events)
E_5cm_ideal = stats.landau.rvs(loc=10.0, scale=1.2, size=N_events)

# Apply realistic detection thresholds & panel efficiencies (e.g., 98% intrinsic efficiency)
thresh_1cm = 0.3  # MeV threshold to trigger a hit
thresh_5cm = 1.0  # MeV threshold to trigger a hit

hit_p1 = (E_1cm_ideal > thresh_1cm) & (np.random.rand(N_events) < 0.98)
hit_p2 = (E_5cm_ideal > thresh_5cm) & (np.random.rand(N_events) < 0.99)
hit_p3 = (E_1cm_ideal > thresh_1cm) & (np.random.rand(N_events) < 0.97)

# Mask energy data: if no hit was registered, energy read is 0
E_p1 = np.where(hit_p1, E_1cm_ideal, 0.0)
E_p2 = np.where(hit_p2, E_5cm_ideal, 0.0)
E_p3 = np.where(hit_p3, E_1cm_ideal, 0.0)

# Mock Angular Data: Let's assume this analysis is repeated for several angles
angles = np.array([0, 10, 20, 30, 40, 50, 60])
# Geometric acceptance drops as cos(theta) due to the 20x20cm area projection
mock_angular_efficiencies = 0.98 * np.cos(np.radians(angles))

# ==========================================
# 2. CORE PERFORMANCE ANALYSIS
# ==========================================

print("=== DETECTOR PERFORMANCE ANALYSIS REPORT ===")
print(f"Total Simulated Injected Muons: {N_events}\n")

# --- A. Detection Efficiency ---
eff_p1 = np.sum(hit_p1) / N_events
eff_p2 = np.sum(hit_p2) / N_events
eff_p3 = np.sum(hit_p3) / N_events

print("--- 1. Panel Single Efficiencies ---")
print(f"Panel 1 (Top, 1 cm):    {eff_p1*100:.2f}%")
print(f"Panel 2 (Middle, 5 cm): {eff_p2*100:.2f}%")
print(f"Panel 3 (Bottom, 1 cm): {eff_p3*100:.2f}%\n")

# --- B. Coincidence Logic ---
# Double coincidences
coinc_1_2 = np.sum(hit_p1 & hit_p2)
coinc_2_3 = np.sum(hit_p2 & hit_p3)
coinc_1_3 = np.sum(hit_p1 & hit_p3)

# Triple coincidence
coinc_triple = np.sum(hit_p1 & hit_p2 & hit_p3)

print("--- 2. Coincidence Matrix ---")
print(f"Double Coincidence (P1 & P2): {coinc_1_2} events ({coinc_1_2/N_events*100:.2f}%)")
print(f"Double Coincidence (P2 & P3): {coinc_2_3} events ({coinc_2_3/N_events*100:.2f}%)")
print(f"Double Coincidence (P1 & P3): {coinc_1_3} events ({coinc_1_3/N_events*100:.2f}%)")
print(f"Triple Coincidence (P1 & P2 & P3): {coinc_triple} events ({coinc_triple/N_events*100:.2f}%)\n")

# --- C. Energy Deposition Metrics ---
# Filter out 0s (non-hits) to calculate physics properties of registered particles
mean_E_p1 = np.mean(E_p1[E_p1 > 0])
mean_E_p2 = np.mean(E_p2[E_p2 > 0])
ratio_5cm_1cm = mean_E_p2 / mean_E_p1

print("--- 3. Energy Deposition Profiles ---")
print(f"P1 (1 cm) Mean Deposited Energy: {mean_E_p1:.3f} MeV")
print(f"P2 (5 cm) Mean Deposited Energy: {mean_E_p2:.3f} MeV")
print(f"Scaling Factor (5 cm / 1 cm):    {ratio_5cm_1cm:.2f}x (Expected ~5.00x)\n")

# --- D. Angular Response Summary ---
print("--- 4. Angular Geometric Response ---")
for angle, eff in zip(angles, mock_angular_efficiencies):
    print(f"Incident Angle: {angle:2d}° | Relative Transmission Efficiency: {eff*100:.2f}%")
