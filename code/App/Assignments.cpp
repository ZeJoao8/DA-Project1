//
// Created by user on 23/03/2026.
//

#include "Assignments.h"
#include "../data_structures/Graph.h"
#include "../algorithms/EdmondsKarp.h"
#include "../structs/Match.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

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
        addFlowEdge(myGraph, SUPER_SOURCE, i, maxRev);

        topicReviewers[reviewers[i].primaryExpertise].push_back(i);
    }

    for (int j = 0; j < submissions.size(); j++) {
        int graphId = j + numReviewers;

        myGraph.addVertex(graphId);
        addFlowEdge(myGraph, graphId, SUPER_SINK, minRev);

        for (int topicReviewerGraphId : topicReviewers[submissions[j].primaryDomain]) {
            addFlowEdge(myGraph, topicReviewerGraphId, graphId, 1);
        }
    }

    return myGraph;
}

void executeReviewAssignment(DataLoader& loader, const string& outputFile) {
    cout << "Executing Review Assignment...\n";
    Graph<int> network = createOnlyFirstDomains(loader);
    double maxFlow = edmondsKarp(&network, SUPER_SOURCE, SUPER_SINK);

    auto& reviewers = loader.getReviewers();
    auto& submissions = loader.getSubmissions();
    int minRev = loader.getMinReviewsPerSubmission();
    int expectedFlow = submissions.size() * minRev;

    vector<Match> assignments;
    for (size_t i = 0; i < reviewers.size(); i++) {
        Vertex<int>* v = network.findVertex(i);
        if (v == nullptr) continue;
        for (Edge<int>* e : v->getAdj()) {
            int destInfo = e->getDest()->getInfo();
            if (destInfo >= (int)reviewers.size() && destInfo < (int)(reviewers.size() + submissions.size())) {
                if (e->getFlow() > 0) {
                    int subIndex = destInfo - reviewers.size();
                    assignments.push_back({reviewers[i].id, submissions[subIndex].id, submissions[subIndex].primaryDomain});
                }
            }
        }
    }

    struct Missing {
        int subId;
        int domain;
        int missingCount;
    };
    vector<Missing> missingReviews;

    if (maxFlow < expectedFlow) {
        for (size_t j = 0; j < submissions.size(); j++) {
            Vertex<int>* v = network.findVertex(reviewers.size() + j);
            if (v != nullptr) {
                for (Edge<int>* e : v->getAdj()) {
                    if (e->getDest()->getInfo() == SUPER_SINK) {
                        if (e->getFlow() < e->getWeight()) {
                            missingReviews.push_back({
                                submissions[j].id,
                                submissions[j].primaryDomain,
                                (int)(e->getWeight() - e->getFlow())
                            });
                        }
                    }
                }
            }
        }
    }

    string outFilename = outputFile.empty() ? loader.getOutputFileName() : outputFile;

    ofstream outFile(outFilename);
    if (outFile.is_open()) {
        sort(assignments.begin(), assignments.end(), [](const Match& a, const Match& b) {
            if (a.subId != b.subId) return a.subId < b.subId;
            return a.revId < b.revId;
        });

        outFile << "#SubmissionId,ReviewerId,Match\n";
        for (const auto& match : assignments) {
            outFile << match.subId << ", " << match.revId << ", " << match.match << "\n";
        }

        sort(assignments.begin(), assignments.end(), [](const Match& a, const Match& b) {
            if (a.revId != b.revId) return a.revId < b.revId;
            return a.subId < b.subId;
        });

        outFile << "#ReviewerId,SubmissionId,Match\n";
        for (const auto& match : assignments) {
            outFile << match.revId << ", " << match.subId << ", " << match.match << "\n";
        }

        outFile << "#Total: " << assignments.size() << "\n";

        if (!missingReviews.empty()) {
            sort(missingReviews.begin(), missingReviews.end(), [](const Missing& a, const Missing& b) {
                return a.subId < b.subId;
            });
            outFile << "#SubmissionId,Domain,MissingReviews\n";
            for (const auto& m : missingReviews) {
                outFile << m.subId << ", " << m.domain << ", " << m.missingCount << "\n";
            }
        }

        outFile.close();
        cout << "Output saved to " << outFilename << "\n";
    }

    if (maxFlow < expectedFlow) {
        cout << "\n--- Assignment Process Failed ---\n";
        cout << "Max Flow (" << maxFlow << ") is less than Expected Flow (" << expectedFlow << ").\n";
        cout << "Failure details for submissions that did not meet the review requirements have been written to " << outFilename << ".\n";
    }
}

template <class T>
class EdgeAccessor : public Edge<T> {
public:
    static void setWeight(Edge<T>* edge, double w) {
        static_cast<EdgeAccessor<T>*>(edge)->weight = w;
    }
};

void executeRiskAnalysis(DataLoader& loader, const string& outputFile) {
    if (loader.getRiskAnalysis() == 0) {
        cout << "Risk analysis disabled.\n";
        return;
    }

    cout << "Executing Risk Analysis...\n";
    Graph<int> network = createOnlyFirstDomains(loader);
    int expectedFlow = loader.getSubmissions().size() * loader.getMinReviewsPerSubmission();
    auto& reviewers = loader.getReviewers();

    vector<int> criticalReviewers;
    Vertex<int>* src = network.findVertex(SUPER_SOURCE);

    for (size_t i = 0; i < reviewers.size(); i++) {
        Edge<int>* targetEdge = nullptr;
        for (Edge<int>* e : src->getAdj()) {
            if (e->getDest()->getInfo() == (int)i) {
                targetEdge = e;
                break;
            }
        }

        if (targetEdge) {
            double originalCapacity = targetEdge->getWeight();
            EdgeAccessor<int>::setWeight(targetEdge, 0);

            double flow = edmondsKarp(&network, SUPER_SOURCE, SUPER_SINK);
            if (flow < expectedFlow) {
                criticalReviewers.push_back(reviewers[i].id);
            }

            EdgeAccessor<int>::setWeight(targetEdge, originalCapacity);
        }
    }

    string outFilename = outputFile.empty() ? loader.getOutputFileName() : outputFile;

    ifstream checkFile(outFilename);
    bool alreadyAppended = false;
    if (checkFile.is_open()) {
        string line;
        while (getline(checkFile, line)) {
            if (line.find("#Risk Analysis:") == 0) {
                alreadyAppended = true;
                break;
            }
        }
        checkFile.close();
    }

    if (alreadyAppended) {
        cout << "Risk analysis complete. (Results were already present in " << outFilename << ", skipping duplicate append)\n";
        return;
    }

    ofstream outFile(outFilename, ios::app);
    if (outFile.is_open()) {
        outFile << "#Risk Analysis: " << loader.getRiskAnalysis() << "\n";
        for (size_t k = 0; k < criticalReviewers.size(); k++) {
            outFile << criticalReviewers[k];
            if (k < criticalReviewers.size() - 1) outFile << ", ";
        }
        outFile << "\n";
        outFile.close();
        cout << "Risk analysis complete. Critical reviewers appended to " << outFilename << "\n";
    } else {
        cerr << "Error: Could not open " << outFilename << " for writing.\n";
    }
}