//
// Created by user on 23/03/2026.
//

#ifndef BATCHMODE_H
#define BATCHMODE_H

#include <string>

/**
 * @file BatchMode.h
 * @brief Automates the execution of the conference organization tool (Task T1.1).
 *
 * Provides functionality to run the program without human interaction via CLI arguments.
 */

/**
 * @brief Executes the review assignment and risk analysis in batch mode.
 *
 * Loads the data from the provided input file, runs the assignment algorithm
 * (and risk analysis, if enabled by the input configuration), and writes
 * the results directly to the specified output file.
 *
 * @param inputFilename      The path to the input configuration CSV file.
 * @param riskOutputFilename The path to the output file to write results and risk analysis.
 * @return                   0 on successful execution, 1 on failure (e.g., file not found).
 *
 * @note Time complexity: Dominated by the execution of the assignment and risk analysis 
 * pipelines, bounded by O(R * V * E²) in the worst-case scenario.
 */
int runBatchMode(const std::string& inputFilename, const std::string& riskOutputFilename);

#endif // BATCHMODE_H