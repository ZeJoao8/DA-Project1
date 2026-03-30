#include "DataLoader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

// Removes leading and trailing whitespace, tabs and newline characters
std::string DataLoader::trim(const std::string &str) const {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (std::string::npos == first) {
    return ""; // String is entirely whitespace
  }
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

// Splits a string by the given delimiter and trims each resulting token
std::vector<std::string> DataLoader::split(const std::string &str,
                                           char delimiter) const {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(str);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(trim(token));
  }
  return tokens;
}

bool DataLoader::loadData(const std::string &filename) {
  // Clear any previously loaded data before parsing a new file
  submissions.clear();
  reviewers.clear();
  parameters.clear();
  controlData.clear();

  // If no path separator is found, assume the file is in the default input directory
  const std::string basePath = "../datasets/input/";
  std::string fullPath;

  if (filename.find('/') != std::string::npos) {
    fullPath = filename;
  } else {
    fullPath = basePath + filename;
  }

  std::ifstream file(fullPath);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << fullPath << "\n";
    return false;
  }

  std::string line;
  enum Section { NONE, SUBMISSIONS, REVIEWERS, PARAMETERS, CONTROL };
  Section currentSection = NONE;

  while (std::getline(file, line)) {
    std::string trimmedLine = trim(line);
    if (trimmedLine.empty())
      continue;

    // Lines starting with '#' are either section headers or comments — skip data parsing
    // Section headers update the current parsing context
    if (trimmedLine[0] == '#') {
      if (trimmedLine == "#Submissions")
        currentSection = SUBMISSIONS;
      else if (trimmedLine == "#Reviewers")
        currentSection = REVIEWERS;
      else if (trimmedLine == "#Parameters")
        currentSection = PARAMETERS;
      else if (trimmedLine == "#Control")
        currentSection = CONTROL;
      // All other # lines (column headers, separators, comments) are ignored
      continue;
    }

    std::vector<std::string> tokens = split(trimmedLine, ',');

    // Dispatch to the appropriate parser based on which section we are in
    switch (currentSection) {
    case SUBMISSIONS:
      parseSubmission(tokens);
      break;
    case REVIEWERS:
      parseReviewer(tokens);
      break;
    case PARAMETERS:
      parseParameter(tokens);
      break;
    case CONTROL:
      parseControl(tokens);
      break;
    case NONE:
      break;
    }
  }

  file.close();
  return true;
}

// Parses a submission line: Id, Title, Authors, Email, Primary, [Secondary]
void DataLoader::parseSubmission(const std::vector<std::string> &tokens) {
  if (tokens.size() < 5) {
    std::cerr << "Error parsing Submission: not enough fields.\n";
    return;
  }
  try {
    int id = std::stoi(tokens[0]);
    std::string title   = tokens[1];
    std::string authors = tokens[2];
    std::string email   = tokens[3];
    int pDomain = std::stoi(tokens[4]);
    int sDomain = -1; // Secondary domain is optional; -1 means not set
    if (tokens.size() > 5 && !tokens[5].empty())
      sDomain = std::stoi(tokens[5]);
    submissions.push_back(Submission{id, title, authors, email, pDomain, sDomain});
  } catch (const std::exception &e) {
    std::cerr << "Error parsing Submission: " << e.what() << "\n";
  }
}

// Stores a key-value parameter (e.g. "MinReviewsPerSubmission, 3")
void DataLoader::parseParameter(const std::vector<std::string> &tokens) {
  if (tokens.size() >= 2) {
    parameters[tokens[0]] = tokens[1];
  }
}

// Stores a control line as a vector of tokens for later lookup
void DataLoader::parseControl(const std::vector<std::string> &tokens) {
  if (!tokens.empty()) {
    controlData.push_back(tokens);
  }
}

// Parses a reviewer line: Id, Name, Email, Primary, [Secondary]
void DataLoader::parseReviewer(const std::vector<std::string> &tokens) {
  if (tokens.size() < 4) {
    std::cerr << "Error parsing Reviewer: not enough fields.\n";
    return;
  }
  try {
    int id = std::stoi(tokens[0]);
    std::string name  = tokens[1];
    std::string email = tokens[2];
    int pExpertise = std::stoi(tokens[3]);
    int sExpertise = -1; // Secondary expertise is optional; -1 means not set
    if (tokens.size() > 4 && !tokens[4].empty())
      sExpertise = std::stoi(tokens[4]);
    reviewers.push_back(Reviewer{id, name, email, pExpertise, sExpertise});
  } catch (const std::exception &e) {
    std::cerr << "Error parsing Reviewer: " << e.what() << "\n";
  }
}

const std::vector<Submission> &DataLoader::getSubmissions() const {
  return submissions;
}

const std::vector<Reviewer> &DataLoader::getReviewers() const {
  return reviewers;
}

const std::map<std::string, std::string> &DataLoader::getParameters() const {
  return parameters;
}

const std::vector<std::vector<std::string>> &DataLoader::getControlData() const {
  return controlData;
}

int DataLoader::getMinReviewsPerSubmission() const {
  auto it = parameters.find("MinReviewsPerSubmission");
  if (it != parameters.end()) {
    try {
      return std::stoi(it->second);
    } catch (...) {
    }
  }
  return 0; // Default if parameter is missing or invalid
}

int DataLoader::getMaxReviewsPerReviewer() const {
  auto it = parameters.find("MaxReviewsPerReviewer");
  if (it != parameters.end()) {
    try {
      return std::stoi(it->second);
    } catch (...) {
    }
  }
  return 0; // Default if parameter is missing or invalid
}

std::string DataLoader::getOutputFileName() const {
  for (const auto &tokens : controlData) {
    if (tokens.size() >= 2 && tokens[0] == "OutputFileName") {
      std::string filename = tokens[1];
      // Strip surrounding quotes if present (e.g. "output.csv" -> output.csv)
      if (filename.size() >= 2 && filename.front() == '"' &&
          filename.back() == '"') {
        filename = filename.substr(1, filename.size() - 2);
      }
      return filename;
    }
  }
  return "output.csv"; // Default output filename
}

int DataLoader::getRiskAnalysis() const {
  for (const auto &tokens : controlData) {
    if (tokens.size() >= 2 && tokens[0] == "RiskAnalysis") {
      try {
        return std::stoi(tokens[1]);
      } catch (...) {
      }
    }
  }
  return 0; // Default: no risk analysis
}

int DataLoader::getGenerateAssignments() const {
  for (const auto &tokens : controlData) {
    if (tokens.size() >= 2 && tokens[0] == "GenerateAssignments") {
      try {
        return std::stoi(tokens[1]);
      } catch (...) {
      }
    }
  }
  return 0; // Default: run but do not report
}