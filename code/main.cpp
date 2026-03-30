/**
 * @file main.cpp
 * @brief Ponto de entrada da aplicação (Entry point).
 *
 * Gere a inicialização do programa, alternando entre o Modo Interativo (CLI) 
 * e o Modo Batch dependendo dos argumentos passados na linha de comandos.
 */

#include <iostream>
#include <string>
#include "data_structures/Graph.h"
#include "utils/DataLoader.h"
#include "structs/Match.h"
#include "algorithms/EdmondsKarp.h"
#include "App/Menu.h"
#include "App/BatchMode.h"

using namespace std;

/**
 * @brief Função principal do programa.
 * * @param argc Número de argumentos da linha de comandos.
 * @param argv Vetor de strings com os argumentos.
 * @return 0 em caso de sucesso, 1 em caso de erro nos argumentos.
 */
int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (argc == 4 && string(argv[1]) == "-b") {
            string inputFilename = argv[2];
            string riskOutputFilename = argv[3];
            return runBatchMode(inputFilename, riskOutputFilename);
        } else {
            cerr << "Batch Usage error: Incorrect arguments provided.\n";
            cerr << "Batch Mode Usage:   " << argv[0] << " -b <input.csv> <risk.csv>\n";
            cerr << "Interactive Usage:  " << argv[0] << "\n";
            return 1;
        }
    }

    runInteractiveMode();

    return 0;
}