
/**
 * @mainpage Scientific Conference Organization Tool
 *
 * @section intro Introduction
 * This project implements a system for organizing scientific conferences,
 * focusing on the automatic assignment of reviewers to submissions.
 *
 * @section problem Problem Description
 * Each submission requires a minimum number of reviews, and each reviewer
 * has a limited capacity and a set of submissions they are able to review.
 *
 * @section approach Approach
 * The problem is modeled as a flow network where:
 * - A Source node connects to reviewers
 * - Reviewers connect to submissions
 * - Submissions connect to the Sink node
 *
 * The Edmonds-Karp algorithm is used to compute the maximum flow,
 * ensuring an optimal assignment of reviewers.
 *
 * @section features Features
 * - Automatic reviewer assignment
 * - Interactive mode (CLI)
 * - Batch mode using CSV files
 * - Risk analysis (detection of critical reviewers)
 *
 * @section authors Authors
 * - Francisco Lourenço - up202404899
 * - Rodrigo Roque – up202403623
 * - José Lopes - up202406360
 *
 * @date 30/03/2026
 */

/**
 * @file main.cpp
 * @brief Ponto de entrada da aplicação (Entry point).
 *
 * Gere a inicialização do programa, alternando entre o Modo Interativo (CLI)
 * e o Modo Batch dependendo dos argumentos passados na linha de comandos.
 */

#include "App/BatchMode.h"
#include "App/Menu.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Função principal do programa.
 * * @param argc Número de argumentos da linha de comandos.
 * @param argv Vetor de strings com os argumentos.
 * @return 0 em caso de sucesso, 1 em caso de erro nos argumentos.
 */
int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (argc == 4 && string(argv[1]) == "-b") {
      string inputFilename = argv[2];
      string riskOutputFilename = argv[3];
      return runBatchMode(inputFilename, riskOutputFilename);
    } else {
      cerr << "Batch Usage error: Incorrect arguments provided.\n";
      cerr << "Batch Mode Usage:   " << argv[0]
           << " -b <input.csv> <risk.csv>\n";
      cerr << "Interactive Usage:  " << argv[0] << "\n";
      return 1;
    }
  }

  runInteractiveMode();

  return 0;
}