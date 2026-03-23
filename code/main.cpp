#include <iostream>
#include <string>
#include "data_structures/Graph.h"
#include "utils/DataLoader.h"
#include "structs/Match.h"
#include "algorithms/EdmondsKarp.h"
#include "App/Menu.h"
#include "App/BatchMode.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Determine execution mode based on arguments supplied
    if (argc > 1) {
        // We expect exact matching for the batch flags
        if (argc == 4 && string(argv[1]) == "-b") {
            string inputFilename = argv[2];
            string riskOutputFilename = argv[3];
            return runBatchMode(inputFilename, riskOutputFilename);
        } else {
            // Arguments provided but malformed according to specifications
            cerr << "Batch Usage error: Incorrect arguments provided.\n";
            cerr << "Batch Mode Usage:   " << argv[0] << " -b <input.csv> <risk.csv>\n";
            cerr << "Interactive Usage:  " << argv[0] << "\n";
            return 1;
        }
    }

    // No arguments supplied, run interactive mode
    runInteractiveMode();

    return 0;
}
