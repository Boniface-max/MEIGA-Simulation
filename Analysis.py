#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

# Import SimDataReader to access data from MEIGA output
from SimDataReader import *


def ConvertBinaryPixel(value, matrix, nBars=12):
    """
    Convert the binary counter information from an eHodoscope
    into a 2D pixel matrix.

    The first nBars bits correspond to X bars and the next
    nBars bits correspond to Y bars.
    """

    # Check for empty or invalid binary data
    if value is None:
        return matrix

    if not isinstance(value, str):
        value = str(value)

    if len(value) < 2 * nBars:
        return matrix

    binary_x = value[0:nBars]
    binary_y = value[nBars:2*nBars]

    # Find pixels where both X and Y bars fired
    for i, itemi in enumerate(binary_x):
        for j, itemj in enumerate(binary_y):

            if (itemi == '1') and (itemj == '1'):
                matrix[i][j] += 1

    return matrix


if __name__ == "__main__":

    import argparse

    parser = argparse.ArgumentParser(
        description="[INFO] This script reads output files from simulations with MEIGA.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )

    parser.add_argument(
        "-f",
        dest="ofile",
        help="Full path to the output file. Example: output.json.gz",
        default=None,
        required=False
    )

    options = parser.parse_args()
    outfile = options.ofile

    print(parser.description)

    if outfile is None:
        print("[ERROR] Path to the output file is needed!")
        exit(1)

    print("[INFO] Reading file %s" % outfile)
    print("\n")

    # ============================================================
    # READ SIMULATION DATA
    # ============================================================

    simData = SimDataReader(outfile)

    # Get list of event IDs
    event_ids = simData.get_event_list()

    print("[INFO] Total number of events in file = %i" % len(event_ids))

    # ============================================================
    # INPUT FLUX
    # ============================================================

    print("[INFO] Accessing data of InputFlux")

    inputFlux = simData.get_input_flux()

    print(inputFlux.head())
    print("\n")

    # ============================================================
    # DETECTOR LIST
    # ============================================================

    print("[INFO] Accessing data from DetectorList")

    detectorList = simData.get_detector_list()

    print(detectorList)

    detector_ids = detectorList.loc['ID']

    print("List of Detector IDs: ", detector_ids)
    print("\n")

    # ============================================================
    # ACCESS THE THREE DETECTORS
    # ============================================================

    print("[INFO] Accessing simulated detector data")

    # Detector 0 = eHodoscope
    detector_0 = simData.GetDetectorSimData(det_id=0)
    edep_0 = detector_0.get_deposited_energy()
    binary_0 = detector_0.get_binary_counter()

    print("[INFO] Detector 0 deposited energy = ", edep_0)
    print("[INFO] Detector 0 binary counter = ", binary_0)

    # Detector 1 = eCounter
    detector_1 = simData.GetDetectorSimData(det_id=1)
    edep_1 = detector_1.get_deposited_energy()

    print("[INFO] Detector 1 deposited energy = ", edep_1)

    # Detector 2 = eHodoscope
    detector_2 = simData.GetDetectorSimData(det_id=2)
    edep_2 = detector_2.get_deposited_energy()
    binary_2 = detector_2.get_binary_counter()

    print("[INFO] Detector 2 deposited energy = ", edep_2)
    print("[INFO] Detector 2 binary counter = ", binary_2)

    print("\n")

    # ============================================================
    # CHECK WHETHER DATA EXIST
    # ============================================================

    if len(inputFlux) == 0:
        print("[WARNING] InputFlux is empty!")
        print("[WARNING] No input particle data were found.")
        print("[WARNING] Check your ARTI/InputFlux file.")
        exit(1)

    if len(edep_0) == 0:
        print("[WARNING] Detector 0 has no deposited-energy data.")

    if len(edep_1) == 0:
        print("[WARNING] Detector 1 has no deposited-energy data.")

    if len(edep_2) == 0:
        print("[WARNING] Detector 2 has no deposited-energy data.")

    # ============================================================
    # MERGE INPUT FLUX AND DETECTOR DATA
    # ============================================================

    print("[INFO] Merging input flux and detector data")

    merged_df = inputFlux.copy()

    # Detector 0: eHodoscope
    merged_df['Detector_0/DepositedEnergy'] = edep_0
    merged_df['Detector_0/BinaryCounter'] = binary_0

    # Detector 1: eCounter
    merged_df['Detector_1/DepositedEnergy'] = edep_1

    # Detector 2: eHodoscope
    merged_df['Detector_2/DepositedEnergy'] = edep_2
    merged_df['Detector_2/BinaryCounter'] = binary_2

    print(merged_df.head())
    print("\n")

    # ============================================================
    # CREATE 12 x 12 MATRICES
    # FOR THE TWO HODOSCOPES
    # ============================================================

    plane_0 = np.zeros((12, 12))
    plane_2 = np.zeros((12, 12))

    # ============================================================
    # THREE-DETECTOR COINCIDENCE
    #
    # Detector 0 = eHodoscope
    # Detector 1 = eCounter
    # Detector 2 = eHodoscope
    #
    # A valid event requires:
    #
    # Detector 0 hit
    # AND
    # Detector 1 hit
    # AND
    # Detector 2 hit
    # ============================================================

    n_events = len(event_ids)

    coincidence_count = 0

    for i in range(n_events):

        # Get data for this event
        binary0 = binary_0[i]
        binary2 = binary_2[i]

        energy1 = edep_1[i]

        # Convert binary counters to temporary matrices
        temp_0 = ConvertBinaryPixel(
            binary0,
            np.zeros((12, 12))
        )

        temp_2 = ConvertBinaryPixel(
            binary2,
            np.zeros((12, 12))
        )

        # Check whether each hodoscope has a hit
        hit_0 = temp_0.sum() > 0
        hit_2 = temp_2.sum() > 0

        # Check whether middle eCounter has deposited energy
        hit_1 = energy1 > 0.1

        # Three-detector coincidence
        if hit_0 and hit_1 and hit_2:

            coincidence_count += 1

            plane_0 += temp_0
            plane_2 += temp_2

    print("[INFO] Number of three-detector coincidence events =",
          coincidence_count)

    # ============================================================
    # PLOT DETECTOR 0
    # ============================================================

    fig = plt.figure()

    im_0 = plt.imshow(
        plane_0,
        interpolation='None',
        cmap=plt.cm.viridis
    )

    plt.colorbar(
        im_0,
        orientation='vertical',
        label='Counts'
    )

    plt.xlabel('Bar number')
    plt.ylabel('Bar number')

    plt.xticks(
        ticks=[1, 3, 5, 7, 9, 11],
        labels=[2, 4, 6, 8, 10, 12]
    )

    plt.yticks(
        ticks=[1, 3, 5, 7, 9, 11],
        labels=[2, 4, 6, 8, 10, 12]
    )

    plt.title('Counts of Detector 0 (eHodoscope)')

    # ============================================================
    # PLOT DETECTOR 2
    # ============================================================

    fig = plt.figure()

    im_2 = plt.imshow(
        plane_2,
        interpolation='None',
        cmap=plt.cm.viridis
    )

    plt.colorbar(
        im_2,
        orientation='vertical',
        label='Counts'
    )

    plt.xlabel('Bar number')
    plt.ylabel('Bar number')

    plt.xticks(
        ticks=[1, 3, 5, 7, 9, 11],
        labels=[2, 4, 6, 8, 10, 12]
    )

    plt.yticks(
        ticks=[1, 3, 5, 7, 9, 11],
        labels=[2, 4, 6, 8, 10, 12]
    )

    plt.title('Counts of Detector 2 (eHodoscope)')

    # ============================================================
    # DEPOSITED ENERGY HISTOGRAM
    # ============================================================

    fig = plt.figure()

    edep_0_array = np.asarray(edep_0)
    edep_1_array = np.asarray(edep_1)
    edep_2_array = np.asarray(edep_2)

    plt.hist(
        edep_0_array[edep_0_array > 0.1],
        bins=50,
        lw=1.8,
        histtype='step',
        label='Detector 0 (eHodoscope)'
    )

    plt.hist(
        edep_1_array[edep_1_array > 0.1],
        bins=50,
        lw=1.8,
        histtype='step',
        label='Detector 1 (eCounter)'
    )

    plt.hist(
        edep_2_array[edep_2_array > 0.1],
        bins=50,
        lw=1.8,
        histtype='step',
        label='Detector 2 (eHodoscope)'
    )

    plt.xlabel('Deposited Energy / MeV')
    plt.ylabel('Counts')
    plt.legend()

    plt.title('Deposited Energy in the Three Detectors')

    plt.show()
