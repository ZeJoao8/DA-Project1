#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::string DataLoader::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> DataLoader::split(const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

void DataLoader::parseSubmission(const std::vector<std::string>& tokens) {
    if (tokens.size() >= 6) {
        try {
            int id = std::stoi(tokens[0]);
            std::string title = tokens[1];
            std::string authors = tokens[2];
            std::string email = tokens[3];
            int pDomain = std::stoi(tokens[4]);
            int sDomain = std::stoi(tokens[5]);
            submissions.emplace_back(id, title, authors, email, pDomain, sDomain);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Submission item: missing or invalid integer format.\n";
        }
    } else {
        std::cerr << "Error parsing Submission: not enough fields.\n";
    }
}

void DataLoader::parseReviewer(const std::vector<std::string>& tokens) {
    if (tokens.size() >= 5) {
        try {
            int id = std::stoi(tokens[0]);
            std::string name = tokens[1];
            std::string email = tokens[2];
            int pExpertise = std::stoi(tokens[3]);
            int sExpertise = std::stoi(tokens[4]);
            reviewers.emplace_back(id, name, email, pExpertise, sExpertise);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Reviewer item: missing or invalid integer format.\n";
        }
    } else {
        std::cerr << "Error parsing Reviewer: not enough fields.\n";
    }
}

void DataLoader::parseParameter(const std::vector<std::string>& tokens) {
    if (tokens.size() >= 2) {
        parameters[tokens[0]] = tokens[1];
    }
}

void DataLoader::parseControl(const std::vector<std::string>& tokens) {
    if (!tokens.empty()) {
        controlData.push_back(tokens);
    }
}

bool DataLoader::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    enum Section { NONE, SUBMISSIONS, REVIEWERS, PARAMETERS, CONTROL };
    Section currentSection = NONE;

    while (std::getline(file, line)) {
        size_t hashPos = line.find('#');
        std::string uncommentedLine = line;
        
        bool isHeader = false;

        std::string trimmedLine = trim(line);
        if (hashPos != std::string::npos) {
            // Check if # is exactly at the start of a recognized header pattern
            if (trimmedLine.length() > 0 && trimmedLine[0] == '#') {
                if (trimmedLine.rfind("#Submissions", 0) == 0) { currentSection = SUBMISSIONS; isHeader = true; }
                else if (trimmedLine.rfind("#Reviewers", 0) == 0) { currentSection = REVIEWERS; isHeader = true; }
                else if (trimmedLine.rfind("#Parameters", 0) == 0) { currentSection = PARAMETERS; isHeader = true; }
                else if (trimmedLine.rfind("#Control", 0) == 0) { currentSection = CONTROL; isHeader = true; }
                
                if (isHeader) {
                    continue; // Skip the rest of the loop to process the next line cleanly
                }
            }
            
            // For any other occurrence of # (inline comment or full-line unrecognized comment),
            // safely truncate the line keeping only characters prior to #
            uncommentedLine = line.substr(0, hashPos);
        }

        uncommentedLine = trim(uncommentedLine);
        
        // Skip purely empty lines resulting from blank lines or fully commented rows
        if (uncommentedLine.empty()) continue;

        std::vector<std::string> tokens = split(uncommentedLine, ',');

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
                // Any line data declared before a proper header is ignored
                break;
        }
    }

    file.close();
    return true;
}

const std::vector<Submission>& DataLoader::getSubmissions() const { return submissions; }
const std::vector<Reviewer>& DataLoader::getReviewers() const { return reviewers; }
const std::map<std::string, std::string>& DataLoader::getParameters() const { return parameters; }
const std::vector<std::vector<std::string>>& DataLoader::getControlData() const { return controlData; }

int DataLoader::getMinReviewsPerSubmission() const {
    auto it = parameters.find("MinReviewsPerSubmission");
    if (it != parameters.end()) {
        try { return std::stoi(it->second); } catch (...) {}
    }
    return 0; // Default fallback
}

int DataLoader::getMaxReviewsPerReviewer() const {
    auto it = parameters.find("MaxReviewsPerReviewer");
    if (it != parameters.end()) {
        try { return std::stoi(it->second); } catch (...) {}
    }
    return 0; // Default fallback
}
