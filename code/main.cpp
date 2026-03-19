#include <iostream>
#include <string>
#include <vector>
#include "data_structures/Graph.h"
#include "utils/DataLoader.h"
#include "structs/Match.h"

using namespace std;

// --- Process Placeholders ---

const int SUPER_SOURCE = -1;
const int SUPER_SINK = -2;


Graph<int> createOnlyFirstDomains(DataLoader& loader){
    //isto so vai funcionar para o generateAssignment a 0 e 1
    //para os outros a criaçao do graph é diferente

    int maxRev = loader.getMaxReviewsPerReviewer();
    int minRev = loader.getMinReviewsPerSubmission();

    auto &reviewers = loader.getReviewers();
    auto &submissions = loader.getSubmissions();
    int numReviewers = reviewers.size();

    map<int, vector<int>> topicReviewers;
    Graph<int> myGraph;
    
    myGraph.addVertex(SUPER_SOURCE); //super source
    myGraph.addVertex(SUPER_SINK); //super sink nao sei se ha melhor forma de fazer isto

    
    for (int i = 0; i < numReviewers; i++){
        myGraph.addVertex(i);
        myGraph.addEdge(SUPER_SOURCE, i, maxRev);

        topicReviewers[reviewers[i].primaryExpertise].push_back(i);
    }

    for (int j = 0; j < submissions.size(); j++) {
        int graphId = j + numReviewers;
        
        myGraph.addVertex(graphId);
        myGraph.addEdge(graphId, SUPER_SINK, minRev);
        
        for (int topicReviewerGraphId : topicReviewers[submissions[j].primaryDomain]) {
            myGraph.addEdge(topicReviewerGraphId, graphId, 1);
        }
    }

    return myGraph;
}



vector<Match> executeReviewAssignment(DataLoader& loader) {
    // TODO: Connect to your Graph data structures and execute the Max Flow algorithms
    // to assign reviewers to submissions here.
    Graph<int> network = createOnlyFirstDomains(loader);



    cout << "Executing Review Assignment...\n";



}

void executeRiskAnalysis(DataLoader& loader, const string& riskOutputFilename) {
    // TODO: Implement the Risk Analysis logic. Output the results to riskOutputFilename
    // if applicable, or process according to project requirements.
    cout << "Executing Risk Analysis...\n";
}

// --- execution modes ---

/**
 * @brief Menu loop for Interactive Mode execution.
 */
void runInteractiveMode() {
    DataLoader loader;
    bool dataLoaded = false;
    int choice = 0;

    cout << "\n============================================\n";
    cout << "   Conference Management System - Interactive\n";
    cout << "============================================\n";

    while (choice != 4) {
        cout << "\nAvailable Options:\n";
        cout << "1. Load Data\n";
        cout << "2. Execute Review Assignment\n";
        cout << "3. Execute Risk Analysis\n";
        cout << "4. Exit\n";
        cout << "Please select an option: ";
        
        if (!(cin >> choice)) {
            // Clear error state from std::cin and ignore bad input
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter the configuration filename (e.g., config.csv): ";
                cin >> filename;
                
                if (loader.loadData(filename)) {
                    cout << "Data loaded successfully!\n";
                    cout << "Loaded " << loader.getSubmissions().size() << " submissions.\n";
                    cout << "Loaded " << loader.getReviewers().size() << " reviewers.\n";
                    dataLoaded = true;
                } else {
                    cout << "Failed to load data. Please check the file path and try again.\n";
                }
                break;
            }
            case 2:
                if (!dataLoaded) {
                    cout << "Error: You must load data first (Option 1).\n";
                } else {
                    executeReviewAssignment(loader);
                }
                break;
            case 3:
                if (!dataLoaded) {
                    cout << "Error: You must load data first (Option 1).\n";
                } else {
                    string file;
                    cout << "Enter output risk file name: ";
                    cin >> file;
                    executeRiskAnalysis(loader, file);
                }
                break;
            case 4:
                cout << "Exiting Interactive Mode...\n";
                break;
            default:
                cout << "Invalid choice. Please select an option between 1 and 4.\n";
                break;
        }
    }
}

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
    executeRiskAnalysis(loader, riskOutputFilename);

    return 0;
}


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
