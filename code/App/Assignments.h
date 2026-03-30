//
// Created by user on 23/03/2026.
//

#ifndef ASSIGNMENTS_H
#define ASSIGNMENTS_H

#include <string>
#include <vector>
#include "../utils/DataLoader.h"
#include "../data_structures/Graph.h"
#include "../structs/Match.h"
#include "../structs/Missing.h"

/**
 * @file Assignments.h
 * @brief Operations for review assignment and risk analysis.
 *
 * Responsoble for the construction of the flow network, execution of the
 * Edmonds-Karp maximum flow algorithm, result extraction, and risk analysis
 * to solve the problem.
 */

/**
 * @brief Constructs the flow network for the reviewer assignment problem.
 *
 * Creates a super source connected to all reviewers (capacity = max reviews)
 * and submissions connected to a super sink (capacity = min reviews). Edges
 * between reviewers and submissions are added based on domain compatibility (@p mode).
 *
 * @param loader The DataLoader object containing parsed CSV data.
 * @param mode   GenerateAssignments value (1 = Primary, 2 = Primary/Secondary, etc.).
 * @return       The constructed flow graph ready for maximum flow computation.
 *
 * @note Time complexity: O(R * S), where R is the number of reviewers and S the number of submissions.
 */
Graph<int> createGraphGeneral(DataLoader &loader, int mode);

/**
 * @brief Extracts successful review assignments from the solved flow network.
 *
 * Traverses the residual graph to find edges between reviewers and submissions
 * that have positive flow, indicating a valid assignment match.
 *
 * @param network The flow network after executing the Edmonds-Karp algorithm.
 * @param loader  The DataLoader object containing original entity data.
 * @param mode    GenerateAssignments value to determine which domain caused the match.
 * @return        A vector of Match structures representing the assignments.
 *
 * @note Time complexity: O(V + E), where V is the number of vertices and E the number of edges.
 */
std::vector<Match> extractAssignments(Graph<int> &network, DataLoader &loader, int mode);


/**
 * @brief Writes the final assignments and missing reviews to a CSV output file.
 *
 * Sorts the assignments first by Submission ID and then by Reviewer ID, and writes them.
 * Appends the incomplete submissions at the end of the file, if any exist.
 *
 * @param filename       The path to the output CSV file.
 * @param assignments    Vector of processed assignment matches.
 * @param missingReviews Vector of submissions that lack the required number of reviews.
 *
 * @note Time complexity: O(M log M + U log U), where M is the number of matches and U the number of unmet submissions.
 */
void writeOutput(const std::string &filename, std::vector<Match> assignments, const std::vector<Missing> &missingReviews);

/**
 * @brief Executes the main pipeline for assigning reviewers to submissions.
 *
 * Builds the flow graph, computes the maximum flow, extracts the assignments
 * and missing reviews, and finally writes the results to the specified output file.
 *
 * @param loader     The DataLoader object populated with input data.
 * @param outputFile Optional custom path for the output file (defaults to loader's config).
 *
 * @note Time complexity: Dominated by the Edmonds-Karp algorithm, O(V * E²).
 */
void executeReviewAssignment(DataLoader& loader, const std::string& outputFile = "");

/**
 * @brief Performs risk analysis (M=1) by testing the system's robustness against reviewer absence.
 *
 * Temporarily disables each reviewer (setting their capacity from the super source to 0)
 * and recalculates the maximum flow. Identifies "critical" reviewers whose absence
 * prevents the system from reaching the expected total flow.
 *
 * @param loader     The DataLoader object populated with input data.
 * @param outputFile Optional custom path to append the risk analysis results.
 *
 * @note Time complexity: O(R * V * E²), where R is the number of reviewers, V is the number of vertices, and E the number of edges.
 */
void executeRiskAnalysis(DataLoader& loader, const std::string& outputFile = "");

/**
 * @brief Identifies submissions that failed to reach the required minimum number of reviews.
 *
 * Checks the edges connecting submissions to the super sink. If the flow is strictly
 * less than the capacity (weight), the submission lacks the necessary reviewers.
 *
 * @param network      The flow network after executing the Edmonds-Karp algorithm.
 * @param loader       The DataLoader object containing original entity data.
 * @param maxFlow      The total maximum flow achieved in the network.
 * @param expectedFlow The ideal total flow (Number of Submissions * Min Reviews).
 * @return             A vector of Missing structures representing submissions lacking reviews.
 *
 * @note Time complexity: O(S + E), where S is the number of submissions and E the number of edges.
 */
std::vector<Missing> findMissingReviews(Graph<int> &network, DataLoader &loader, double maxFlow, int expectedFlow);

#endif // ASSIGNMENTS_H