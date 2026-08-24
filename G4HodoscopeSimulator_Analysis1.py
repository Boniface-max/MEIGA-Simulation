#!/usr/bin/python3
import argparse
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from SimDataReader import *


def ConvertBinaryPixel(value, matrix, nBars=12):
    """
    Parses a 24-bit binary string split into X (first nBars) and Y (last nBars)
    and increments corresponding (x, y) hit bins in the matrix.
    """
    if not isinstance(value, str) or len(value) < 2 * nBars:
        return matrix

    binary_x = value[0:nBars]
    binary_y = value[nBars:2 * nBars]

    for i, itemi in enumerate(binary_x):
        for j, itemj in enumerate(binary_y):
            if itemi == '1' and itemj == '1':
                matrix[i][j] += 1

    return matrix


def HasHit(value, nBars=12):
    """Returns True if both X and Y views register at least one hit ('1')."""
    if not isinstance(value, str) or len(value) < 2 * nBars:
        return False
    binary_x = value[0:nBars]
    binary_y = value[nBars:2 * nBars]
    return ('1' in binary_x) and ('1' in binary_y)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="[INFO] Reads and analyzes MEIGA Hodoscope simulation data.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument("-f", dest="ofile", help="Path to output file (e.g., output.json)", default=None, required=True)

    options = parser.parse_args()
    outfile = options.ofile

    print(f"[INFO] Reading file: {outfile}\n")

    # Accessing Simulated data
    simData = SimDataReader(outfile)
    event_ids = simData.get_event_list()
    n_events = len(event_ids)
    print(f"[INFO] Total number of events = {n_events}")

    if n_events == 0:
        print("[WARNING] No events found in simulation file! Increase primary particle flux/count.")
        exit(0)

    # Get input flux DataFrame
    inputFlux = simData.get_input_flux()
    detectorList = simData.get_detector_list()
    
    # Extract list of detector IDs dynamically
    detector_ids = list(detectorList.loc['ID'])
    print(f"[INFO] Detected IDs in geometry: {detector_ids}")

    merged_df = inputFlux.copy()

    # Merge detector data safely handling varying length/empty hits
    for detId in detector_ids:
        detSimData = simData.GetDetectorSimData(det_id=detId)
        
        edep = detSimData.get_deposited_energy()
        barCounter = detSimData.get_binary_counter()

        # Ensure array lengths match input flux length to avoid Pandas ValueError
        if len(edep) != len(merged_df):
            edep = np.pad(edep, (0, max(0, len(merged_df) - len(edep))), 'constant')[:len(merged_df)]
        if len(barCounter) != len(merged_df):
            barCounter = ["0" * 24] * len(merged_df)

        merged_df[f'Detector_{detId}/DepositedEnergy'] = edep
        merged_df[f'Detector_{detId}/BinaryCounter'] = barCounter

    # Coincidence Analysis for 3 Planes (IDs 0, 1, 2)
    planes = {detId: np.zeros((12, 12)) for detId in detector_ids}

    for i in range(len(merged_df)):
        # Check if 3-plane coincidence requirement is met
        hits = {detId: HasHit(merged_df[f'Detector_{detId}/BinaryCounter'].iloc[i]) for detId in detector_ids}

        # If all available detectors triggered in coincidence:
        if all(hits.values()) and len(detector_ids) > 0:
            for detId in detector_ids:
                binary_val = merged_df[f'Detector_{detId}/BinaryCounter'].iloc[i]
                planes[detId] = ConvertBinaryPixel(binary_val, planes[detId])

    # Plot spatial occupancy heatmaps for each plane
    for detId, matrix in planes.items():
        det_name = detectorList[detId]['Name'] if detId in detectorList else f"Detector {detId}"
        
        plt.figure()
        im = plt.imshow(matrix, interpolation='None', cmap='viridis')
        plt.colorbar(im, orientation='vertical', label='Counts')
        plt.xlabel('Bar Number (X)')
        plt.ylabel('Bar Number (Y)')
        plt.xticks(ticks=range(12), labels=range(1, 13))
        plt.yticks(ticks=range(12), labels=range(1, 13))
        plt.title(f'Spatial Hit Map - {det_name} (ID: {detId})')
        plt.tight_layout()

    # Plot Deposited Energy Distributions
    plt.figure()
    for detId in detector_ids:
        det_name = detectorList[detId]['Name'] if detId in detectorList else f"Detector {detId}"
        edep = merged_df[f'Detector_{detId}/DepositedEnergy']
        
        # Filter non-zero energy hits
        valid_edep = edep[edep > 0.01]
        if len(valid_edep) > 0:
            plt.hist(valid_edep, bins=50, histtype='step', lw=1.8, label=f'{det_name} (ID {detId})')

    plt.xlabel('Deposited Energy / MeV')
    plt.ylabel('Counts')
    plt.title('Energy Deposition Spectrum')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()

    plt.show()
