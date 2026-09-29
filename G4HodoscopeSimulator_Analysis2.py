import numpy as np
import matplotlib
# Uncomment the line below if running inside a headless Docker container:
matplotlib.use('Agg') 
import matplotlib.pyplot as plt
from SimDataReader import *


def ConvertBinaryPixel(value, matrix=None, nBars=12):
    """
    Parses a 2*nBars binary string (X-bars and Y-bars) 
    and increments corresponding matrix pixels.
    """	
    if matrix is None:
        matrix = np.zeros((nBars, nBars))

    binary_x = value[0:nBars]
    binary_y = value[nBars:2*nBars]

    for i, itemi in enumerate(binary_x):
        for j, itemj in enumerate(binary_y):
            if itemi == '1' and itemj == '1':
                matrix[i][j] += 1

    return matrix


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(
        description="[INFO] Reads MEIGA simulation output files and generates hodoscope occupancy maps.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument("-f", dest="ofile", help="Full path to output file (e.g., output.json)", default=None, required=True)
    parser.add_argument("--save", dest="save_fig", action="store_true", help="Save plots as PNG files instead of showing interactive window")

    options = parser.parse_args()
    outfile = options.ofile

    print(f"[INFO] Reading file: {outfile}\n")

    # Access Simulated Data
    simData = SimDataReader(outfile)
    event_ids = simData.get_event_list()
    print(f"[INFO] Total number of events = {len(event_ids)}")

    inputFlux = simData.get_input_flux()
    detectorList = simData.get_detector_list()
    detector_ids = detectorList.loc['ID']

    print("[INFO] Merging input flux and deposited energy data...")
    merged_df = inputFlux.copy()

    for detId in detector_ids:
        detSimData = simData.GetDetectorSimData(det_id=detId)
        merged_df[f'Detector_{detId}/DepositedEnergy'] = detSimData.get_deposited_energy()
        merged_df[f'Detector_{detId}/BinaryCounter'] = detSimData.get_binary_counter()

    # Retrieve binary counters
    binaryCounter_det_0 = merged_df['Detector_0/BinaryCounter']
    binaryCounter_det_1 = merged_df['Detector_1/BinaryCounter']
    binaryCounter_det_2 = merged_df['Detector_2/BinaryCounter']

    # Occupancy matrices
    plane_0 = np.zeros((12, 12))
    plane_1 = np.zeros((12, 12))
    plane_2 = np.zeros((12, 12))

    n_events = len(event_ids)

    # Process coincidence and update planes
    for i in range(n_events):
        b0, b1, b2 = binaryCounter_det_0[i], binaryCounter_det_1[i], binaryCounter_det_2[i]

        # Calculate hits per plane for current event
        hits_0 = ConvertBinaryPixel(b0, np.zeros((12, 12)))
        hits_1 = ConvertBinaryPixel(b1, np.zeros((12, 12)))
        hits_2 = ConvertBinaryPixel(b2, np.zeros((12, 12)))

        # Coincidence condition: Hit registered on all 3 detector planes
        if hits_0.sum() > 0 and hits_1.sum() > 0 and hits_2.sum() > 0:
            plane_0 += hits_0
            plane_1 += hits_1
            plane_2 += hits_2

    # --- Plotting Section ---
    planes = [plane_0, plane_1, plane_2]
    
    for idx, plane in enumerate(planes):
        fig, ax = plt.subplots()
        im = ax.imshow(plane, interpolation='None', cmap=plt.cm.viridis)
        plt.colorbar(im, ax=ax, orientation='vertical', label='Counts')
        ax.set_xlabel('Bar number')
        ax.set_ylabel('Bar number')
        ax.set_xticks(ticks=[1, 3, 5, 7, 9, 11], labels=[2, 4, 6, 8, 10, 12])
        ax.set_yticks(ticks=[1, 3, 5, 7, 9, 11], labels=[2, 4, 6, 8, 10, 12])
        ax.set_title(f'Counts of Detector {idx}')
        
        if options.save_fig:
            plt.savefig(f'detector_{idx}_occupancy.png', bbox_inches='tight')
            print(f"[INFO] Saved detector_{idx}_occupancy.png")

    # Energy deposition histogram
    fig, ax = plt.subplots()
    for detId in detector_ids:
        edep = merged_df[f'Detector_{detId}/DepositedEnergy']
        ax.hist(edep[edep > 0.1], bins=50, lw=1.8, histtype='step', label=f'Detector {detId}')

    ax.set_xlabel('Deposited Energy / MeV')
    ax.set_ylabel('Counts')
    ax.legend()
    
    if options.save_fig:
        plt.savefig('deposited_energy_histogram.png', bbox_inches='tight')
        print("[INFO] Saved deposited_energy_histogram.png")
    else:
        plt.show()

    print("[INFO] Script execution completed successfully.")
