#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include <map>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"

/**
 * @brief Utility class to load configuration data from a custom CSV file.
 * 
 * Supports reading Submissions, Reviewers, Parameters, and Control sets 
 * from predefined sections labeled with headers starting with '#'. 
 * It properly removes inline comments started by '#'.
 */
class DataLoader {
private:
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    std::map<std::string, std::string> parameters;
    std::vector<std::vector<std::string>> controlData;

    /**
     * @brief Helper function to trim whitespace from ends of a string.
     */
    std::string trim(const std::string& str) const;

    /**
     * @brief Helper function to split a string by a given character delimiter.
     */
    std::vector<std::string> split(const std::string& str, char delimiter) const;

    void parseSubmission(const std::vector<std::string>& tokens);
    void parseReviewer(const std::vector<std::string>& tokens);
    void parseParameter(const std::vector<std::string>& tokens);
    void parseControl(const std::vector<std::string>& tokens);

public:
    /**
     * @brief Default constructor.
     */
    DataLoader() = default;

    /**
     * @brief Loads and parses data from the specified setup CSV file.
     * @param filename Path to the configuration CSV file.
     * @return true if successful, false if the file cannot be opened.
     */
    bool loadData(const std::string& filename);

    // Getters for the parsed data collections
    const std::vector<Submission>& getSubmissions() const;
    const std::vector<Reviewer>& getReviewers() const;
    const std::map<std::string, std::string>& getParameters() const;
    const std::vector<std::vector<std::string>>& getControlData() const;
    
    // Convenience getters for common parameters expected logically
    int getMinReviewsPerSubmission() const;
    int getMaxReviewsPerReviewer() const;
    std::string getOutputFileName() const;
    int getRiskAnalysis() const;
};

#endif // DATALOADER_H
