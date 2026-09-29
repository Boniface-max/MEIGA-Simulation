#!/usr/bin/env python3
import sys
import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt  # Added Matplotlib

def process_3panel_hodoscope(base_df=None):
    """
    Processes energy deposition for a 3-panel scintillator stack:
      - Top panel:    eHodoscope (ID 0)
      - Middle panel: eCounter    (ID 1)
      - Bottom panel: eHodoscope (ID 2)
    """
    
    # Define detector mapping: ID -> Name
    detectors = {
        0: "eHodoscope",
        1: "eCounter",
        2: "eHodoscope"
    }
    
    print("[INFO] Processing 3-panel scintillator stack (IDs: 0, 1, 2)...")
    
    # Initialize DataFrame with at least one row index if base_df is not provided
    if base_df is None:
        merged_df = pd.DataFrame(index=[0])
    else:
        merged_df = base_df.copy()
        
    target_len = len(merged_df)
    
    # --- LOAD SIMULATION DATA ---
    try:
        with open("output.json", "r") as f:
            sim_data = json.load(f)
    except FileNotFoundError:
        print("[ERROR] output.json file not found! Defaulting to empty data.")
        sim_data = {}
    # ----------------------------
    
    for det_id, det_name in detectors.items():
        print(f"[INFO] Accessing Simulated data for {det_name} (ID = {det_id})")
        
        # Fetch data from JSON keys (e.g., "0", "1", "2"). If missing, default to empty list.
        edep = sim_data.get(str(det_id), [])
        
        column_name = f'Detector_{det_id}_{det_name}/DepositedEnergy'
        
        # Safely insert hits or default 0.0 without triggering Pandas length errors
        if len(edep) == target_len:
            merged_df[column_name] = edep
        elif len(edep) == 0:
            print(f"[WARN] {det_name} (ID {det_id}) recorded 0 hits. Filling with 0.0 MeV.")
            merged_df[column_name] = 0.0
        else:
            # If data length matches event counts but not target_len, adjust row index
            if target_len == 1:
                merged_df = pd.DataFrame(index=range(len(edep)))
                target_len = len(merged_df)
                merged_df[column_name] = edep
            else:
                merged_df[column_name] = edep[:target_len]
            
    print("[INFO] 3-panel data processing complete.")
    return merged_df

def plot_detector_response(df):
    """Generates a plot to view the energy depositions of all panels."""
    plt.figure(figsize=(10, 6))
    
    # Filter columns that contain detector energy data
    energy_cols = [col for col in df.columns if 'DepositedEnergy' in col]
    
    if not energy_cols:
        print("[WARN] No energy data found to plot.")
        return

    # Plot an energy distribution histogram for each detector panel
    for col in energy_cols:
        label = col.split('/')[0] # Cleans up legend label (e.g., Detector_0_eHodoscope)
        plt.hist(df[col], bins=50, alpha=0.6, label=label, edgecolor='black')
        
    plt.title("3-Panel Scintillator Stack: Energy Deposition Response")
    plt.xlabel("Deposited Energy (MeV)")
    plt.ylabel("Counts / Event Frequency")
    plt.yscale('log') # Useful if you have high background variations or sharp peak discrepancies
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend()
    
    # Save the output visualization image
    plt.savefig("detector_response.png", dpi=300)
    print("[INFO] Plot successfully generated and saved as 'detector_response.png'")
    plt.show()

if __name__ == "__main__":
    try:
        final_df = process_3panel_hodoscope()
        print("\n--- 3-Panel Detector Summary ---")
        print(final_df.head()) # Shows the first few entries cleanly
        
        # Run the visualization tool
        plot_detector_response(final_df)
        
    except Exception as e:
        print(f"[ERROR] Simulation analysis failed: {e}")
        sys.exit(1)
