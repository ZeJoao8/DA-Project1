//
// Created by Zé João on 23/03/2026.
//

#ifndef MENU_H
#define MENU_H

/**
 * @file Menu.h
 * @brief Interactive terminal menu for the conference organization tool.
 *
 * Provides a user-friendly CLI to load datasets, inspect data, and execute
 * the review assignment and risk analysis algorithms step-by-step.
 */

/**
 * @brief Launches the interactive menu loop.
 *
 * Displays a text-based interface allowing the user to input commands,
 * load CSV files, display loaded submissions and reviewers, and trigger
 * the Edmonds-Karp maximum flow algorithm for review assignment or risk analysis.
 * Continues running until the user selects the exit option (0).
 *
 * @note Time complexity: O(1) for menu rendering and basic operations, but bounded 
 * by O(V * E²) when executing the assignment or risk analysis algorithms.
 */
void runInteractiveMode();

#endif // MENU_H