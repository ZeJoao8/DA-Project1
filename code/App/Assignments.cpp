#include "Assignments.h"
#include "../algorithms/EdmondsKarp.h"
#include "../data_structures/Graph.h"
#include "../structs/Match.h"
#include "../structs/Missing.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

// --- Process Placeholders ---

const int SUPER_SOURCE = -1;
const int SUPER_SINK = -2;

Graph<int> createGraphGeneral(DataLoader &loader, int mode) {
  int maxRev = loader.getMaxReviewsPerReviewer();
  int minRev = loader.getMinReviewsPerSubmission();

  auto &reviewers = loader.getReviewers();
  auto &submissions = loader.getSubmissions();
  int numReviewers = reviewers.size();

  Graph<int> myGraph;
  myGraph.addVertex(SUPER_SOURCE);
  myGraph.addVertex(SUPER_SINK);

  for (int i = 0; i < numReviewers; i++) {
    myGraph.addVertex(i);
    addFlowEdge(myGraph, SUPER_SOURCE, i, maxRev);
  }

  for (int j = 0; j < submissions.size(); j++) {
    int subId = j + numReviewers;

    myGraph.addVertex(subId);
    addFlowEdge(myGraph, subId, SUPER_SINK, minRev);

    for (int i = 0; i < numReviewers; i++) {

      bool match = false;

      int rPrim = reviewers[i].primaryExpertise;
      int rSec = reviewers[i].secondaryExpertise;

      int sPrim = submissions[j].primaryDomain;
      int sSec = submissions[j].secondaryDomain;

      if (mode == 1 || mode == 0) {
        match = (rPrim == sPrim);
      }

      else if (mode == 2) {
        match = (rPrim == sPrim) || (sSec != -1 && rPrim == sSec);
      }

      else if (mode == 3) {
        match = (rPrim == sPrim) || (rSec != -1 && rSec == sPrim) ||
                (sSec != -1 && rPrim == sSec) ||
                (sSec != -1 && rSec != -1 && rSec == sSec);
      }

      if (match) {
        addFlowEdge(myGraph, i, subId, 1);
      }
    }
  }

  return myGraph;
}

vector<Match> extractAssignments(Graph<int> &network, DataLoader &loader,
                                 int mode) {
  auto &reviewers = loader.getReviewers();
  auto &submissions = loader.getSubmissions();

  vector<Match> assignments;

  for (size_t i = 0; i < reviewers.size(); i++) {
    Vertex<int> *v = network.findVertex(i);
    if (v == nullptr)
      continue;

    for (Edge<int> *e : v->getAdj()) {
      int destInfo = e->getDest()->getInfo();
      if (destInfo >= (int)reviewers.size() &&
          destInfo < (int)(reviewers.size() + submissions.size())) {
        if (e->getFlow() > 0) {
          int subIndex = destInfo - reviewers.size();

          int subPrim = submissions[subIndex].primaryDomain;
          int subSec = submissions[subIndex].secondaryDomain;
          int revPrim = reviewers[i].primaryExpertise;
          int revSec = reviewers[i].secondaryExpertise;

          int matchDomain = -1;
          if (revPrim == subPrim) {
            matchDomain = subPrim;
          } else if (mode >= 2 && revPrim == subSec) {
            matchDomain = subSec;
          } else if (mode == 3 && revSec == subPrim) {
            matchDomain = subPrim;
          } else if (mode == 3 && revSec == subSec) {
            matchDomain = subSec;
          }

          assignments.push_back(
              {reviewers[i].id, submissions[subIndex].id, matchDomain});
        }
      }
    }
  }

  return assignments;
}

vector<Missing> findMissingReviews(Graph<int> &network, DataLoader &loader,
                                   double maxFlow, int expectedFlow) {
  auto &reviewers = loader.getReviewers();
  auto &submissions = loader.getSubmissions();

  vector<Missing> missingReviews;

  if (maxFlow < expectedFlow) {
    for (size_t j = 0; j < submissions.size(); j++) {
      Vertex<int> *v = network.findVertex(reviewers.size() + j);
      if (v == nullptr)
        continue;

      for (Edge<int> *e : v->getAdj()) {
        if (e->getDest()->getInfo() == SUPER_SINK) {
          if (e->getFlow() < e->getWeight()) {
            missingReviews.push_back({submissions[j].id,
                                      submissions[j].primaryDomain,
                                      (int)(e->getWeight() - e->getFlow())});
          }
        }
      }
    }
  }

  return missingReviews;
}

void writeOutput(const string &filename, vector<Match> assignments,
                 const vector<Missing> &missingReviews) {
  ofstream outFile(filename);
  if (!outFile.is_open()) {
    cerr << "Error: Could not open " << filename << " for writing.\n";
    return;
  }

  sort(assignments.begin(), assignments.end(),
       [](const Match &a, const Match &b) {
         if (a.subId != b.subId)
           return a.subId < b.subId;
         return a.revId < b.revId;
       });

  outFile << "#SubmissionId,ReviewerId,Match\n";
  for (const auto &match : assignments) {
    outFile << match.subId << ", " << match.revId << ", " << match.match
            << "\n";
  }

  sort(assignments.begin(), assignments.end(),
       [](const Match &a, const Match &b) {
         if (a.revId != b.revId)
           return a.revId < b.revId;
         return a.subId < b.subId;
       });

  outFile << "#ReviewerId,SubmissionId,Match\n";
  for (const auto &match : assignments) {
    outFile << match.revId << ", " << match.subId << ", " << match.match
            << "\n";
  }

  outFile << "#Total: " << assignments.size() << "\n";

  if (!missingReviews.empty()) {
    vector<Missing> sortedMissing = missingReviews;
    sort(sortedMissing.begin(), sortedMissing.end(),
         [](const Missing &a, const Missing &b) { return a.subId < b.subId; });

    outFile << "#SubmissionId,Domain,MissingReviews\n";
    for (const auto &m : sortedMissing) {
      outFile << m.subId << ", " << m.domain << ", " << m.missingCount << "\n";
    }
  }

  outFile.close();
}

void executeReviewAssignment(DataLoader &loader, const string &outputFile) {

  int mode = loader.getGenerateAssignments();
  Graph<int> network = createGraphGeneral(loader, mode);
  double maxFlow = edmondsKarp(&network, SUPER_SOURCE, SUPER_SINK);

  if (mode == 0)
    return;

  cout << "Executing Review Assignment (Mode " << mode << ")...\n";

  auto &submissions = loader.getSubmissions();
  int expectedFlow = submissions.size() * loader.getMinReviewsPerSubmission();

  vector<Match> assignments = extractAssignments(network, loader, mode);
  vector<Missing> missingReviews =
      findMissingReviews(network, loader, maxFlow, expectedFlow);

  string outFilename =
      outputFile.empty() ? loader.getOutputFileName() : outputFile;
  writeOutput(outFilename, assignments, missingReviews);

  if (maxFlow < expectedFlow) {
    cout << "\n--- Assignment Process Failed ---\n";
    cout << "Max Flow (" << maxFlow << ") is less than Expected Flow ("
         << expectedFlow << ").\n";
    cout << "Failure details written to " << outFilename << ".\n";
  } else {
    cout << "\n--- Assignment Process Completed Successfully ---\n";
    cout << "Output saved to " << outFilename << "\n";
  }
}

void executeRiskAnalysis(DataLoader &loader, const string &outputFile) {
  if (loader.getRiskAnalysis() == 0) {
    cout << "Risk analysis disabled.\n";
    return;
  }

  int mode = loader.getGenerateAssignments();

  cout << "Executing Risk Analysis (Mode " << mode << ")...\n";
  Graph<int> network = createGraphGeneral(loader, mode);
  int expectedFlow =
      loader.getSubmissions().size() * loader.getMinReviewsPerSubmission();
  auto &reviewers = loader.getReviewers();

  vector<int> criticalReviewers;
  Vertex<int> *src = network.findVertex(SUPER_SOURCE);

  for (size_t i = 0; i < reviewers.size(); i++) {
    Edge<int> *targetEdge = nullptr;
    for (Edge<int> *e : src->getAdj()) {
      if (e->getDest()->getInfo() == (int)i) {
        targetEdge = e;
        break;
      }
    }

    if (targetEdge) {
      double originalCapacity = targetEdge->getWeight();
      targetEdge->setWeight(0);

      double flow = edmondsKarp(&network, SUPER_SOURCE, SUPER_SINK);
      if (flow < expectedFlow) {
        criticalReviewers.push_back(reviewers[i].id);
      }

      targetEdge->setWeight(originalCapacity);
    }
  }

  string outFilename =
      outputFile.empty() ? loader.getOutputFileName() : outputFile;

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
    cout << "Risk analysis complete. (Results were already present in "
         << outFilename << ", skipping duplicate append)\n";
    return;
  }

  ofstream outFile(outFilename, ios::app);
  if (outFile.is_open()) {
    outFile << "#Risk Analysis: " << loader.getRiskAnalysis() << "\n";
    for (size_t k = 0; k < criticalReviewers.size(); k++) {
      outFile << criticalReviewers[k];
      if (k < criticalReviewers.size() - 1)
        outFile << ", ";
    }
    outFile << "\n";
    outFile.close();
    cout << "Risk analysis complete. Critical reviewers appended to "
         << outFilename << "\n";
  } else {
    cerr << "Error: Could not open " << outFilename << " for writing.\n";
  }
}