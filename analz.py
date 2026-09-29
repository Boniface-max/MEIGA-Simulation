#!/usr/bin/env python3
import sys
import numpy as np
import pandas as pd

def process_3panel_hodoscope(base_df=None):
    """
    Processes energy deposition for a 3-panel scintillator stack:
      - Top panel:    eHodoscope (ID 0)
      - Middle panel: eCounter   (ID 1)
      - Bottom panel: eHodoscope (ID 2)
    """
    # Define detector mapping: ID -> Name
    detectors = {
        0: "eHodoscope",
        1: "eCounter",
        2: "eHodoscope"
    }

    print("[INFO] Processing 3-panel scintillator stack (IDs: 0, 1, 2)...")

    if base_df is None:
        merged_df = pd.DataFrame(index=[0])
    else:
        merged_df = base_df.copy()

    target_len = len(merged_df)

    for det_id, det_name in detectors.items():
        print(f"[INFO] Accessing Simulated data for {det_name} (ID = {det_id})")

        # Fetch/extract energy deposition array for current panel
        # Replace [] with your file parsing logic (e.g., from output.json)
        edep = []  

        column_name = f'Detector_{det_id}_{det_name}/DepositedEnergy'

        # Safely insert hits or default 0.0 without triggering Pandas length errors
        if len(edep) == target_len:
            merged_df[column_name] = edep
        elif len(edep) == 0:
            print(f"[WARN] {det_name} (ID {det_id}) recorded 0 hits. Filling with 0.0 MeV.")
            merged_df[column_name] = 0.0
        else:
            merged_df[column_name] = [edep] * target_len

    print("[INFO] 3-panel data processing complete.")
    return merged_df


if __name__ == "__main__":
    try:
        final_df = process_3panel_hodoscope()
        print("\n--- 3-Panel Detector Summary ---")
        print(final_df)
    except Exception as e:
        print(f"[ERROR] Simulation analysis failed: {e}")
        sys.exit(1)
