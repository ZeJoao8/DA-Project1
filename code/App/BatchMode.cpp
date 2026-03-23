//
// Created by user on 23/03/2026.
//
#include "BatchMode.h"
#include "Assignments.h"
#include "../utils/DataLoader.h"
#include <iostream>

using namespace std;
/**
 * @brief Batch Mode execution without human interaction.
 * @param inputFilename The input configuration CSV file.
 * @param riskOutputFilename The file to write risk analysis results.
 * @return 0 on success, non-zero on failure.
 */

int runBatchMode(const string& inputFilename, const string& riskOutputFilename) {
    DataLoader loader;

    // DataLoader currently logs errors natively to std::cerr.
    if (!loader.loadData(inputFilename)) {
        cerr << "Batch Mode Error: Failed to load input configuration file: " << inputFilename << endl;
        return 1;
    }

    // Execute functions without interactive printbacks (you could suppress standard cout here)
    executeReviewAssignment(loader);
    executeRiskAnalysis(loader);

    return 0;
}