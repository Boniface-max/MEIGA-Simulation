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
    # Example placeholder: replace with your actual hit extraction logic
    # edep = get_hits_from_json_or_root(det_id)
    edep = []  # Simulated empty return case that previously triggered ValueError
    return edep

def process_and_merge_simulation_data(detector_ids, base_df=None):
    """
    Safely processes detector deposited energy arrays and merges them
    into the primary analysis DataFrame without triggering length mismatch errors.
    """
    print("[INFO] Starting simulation data analysis...")

    # Initialize a base DataFrame if one isn't passed in
    if base_df is None:
        # Assuming 1 row per primary event simulated (adjust index size if needed)
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

        # SAFE MERGING LOGIC (Fixes line 86 ValueError)
        if len(edep) == target_len:
            # Case 1: Length matches perfectly
            merged_df[column_name] = edep
        elif len(edep) == 0:
            # Case 2: No energy deposited / empty array -> Assign default 0.0
            print(f"[WARN] Detector {det_id} recorded 0 hits. Filling with default 0.0 MeV.")
            merged_df[column_name] = 0.0
        elif isinstance(edep, (list, np.ndarray)) and len(edep) != target_len:
            # Case 3: Event multi-hits or array size mismatch -> Store as object/list column
            print(f"[INFO] Array length mismatch ({len(edep)} vs DataFrame {target_len}). Storing raw list object.")
            merged_df[column_name] = [edep] * target_len
        else:
            # Case 4: Scalar value fallback
            merged_df[column_name] = edep

    print("[INFO] Merging input flux and deposited energy data complete.")[cite: 1]
    return merged_df

if __name__ == "__main__":
    # Detector IDs defined in DetectorList (e.g., Detector_0 and Detector_1)
    detector_ids = [0, 1][cite: 1]
    
    # Run analysis safely
    try:
        final_df = process_and_merge_simulation_data(detector_ids)
        print("\n--- Final Analysis DataFrame ---")
        print(final_df)
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")
        sys.exit(1)
