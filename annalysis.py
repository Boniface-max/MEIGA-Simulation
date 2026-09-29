#!/usr/bin/env python3
import os
import sys
import numpy as np
import pandas as pd

def load_detector_data(det_id):
    """
    Simulated function to fetch hit data for a given detector ID.
    Replace or adapt this function body to match your exact file loading logic.
    """
    edep = []  # Returns empty array when no energy deposition is recorded
    return edep

def process_and_merge_simulation_data(detector_ids, base_df=None):
    """
    Safely processes detector deposited energy arrays and merges them
    into the primary analysis DataFrame without triggering length mismatch errors.
    """
    print("[INFO] Starting simulation data analysis...")

    # Initialize a base DataFrame if one isn't passed in
    if base_df is None:
        merged_df = pd.DataFrame(index=[0])
    else:
        merged_df = base_df.copy()

    for det_id in detector_ids:
        print(f"[INFO] Accessing Simulated data of Detector with ID = {det_id}")[cite: 1]
        
        # Extract deposited energy array for current detector
        edep = load_detector_data(det_id)
        print(f"[INFO] Deposited energy = {edep}")[cite: 1]

        column_name = f'Detector_{det_id}/DepositedEnergy'[cite: 1]
        target_len = len(merged_df)

        # SAFE MERGING LOGIC
        if len(edep) == target_len:
            merged_df[column_name] = edep
        elif len(edep) == 0:
            print(f"[WARN] Detector {det_id} recorded 0 hits. Filling with default 0.0 MeV.")
            merged_df[column_name] = 0.0
        elif isinstance(edep, (list, np.ndarray)) and len(edep) != target_len:
            print(f"[INFO] Array length mismatch ({len(edep)} vs DataFrame {target_len}). Storing raw list object.")
            merged_df[column_name] = [edep] * target_len
        else:
            merged_df[column_name] = edep

    print("[INFO] Merging input flux and deposited energy data complete.")[cite: 1]
    return merged_df

if __name__ == "__main__":
    detector_ids = [0, 1]
    
    try:
        final_df = process_and_merge_simulation_data(detector_ids)
        print("\n--- Final Analysis DataFrame ---")
        print(final_df)
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")
        sys.exit(1)
