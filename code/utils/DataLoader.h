#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include <map>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"

/**
 * @file DataLoader.h
 * @brief Handles reading and parsing of the input configuration files (Task T1.2).
 */

/**
 * @class DataLoader
 * @brief Utility class to load configuration data from a custom CSV file.
 * * Supports reading Submissions, Reviewers, Parameters, and Control sets 
 * from predefined sections labeled with headers starting with '#'. 
 * Extracts data into ready-to-use C++ data structures.
 */
class DataLoader {
private:
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    std::map<std::string, std::string> parameters;
    std::vector<std::vector<std::string>> controlData;

    /**
     * @brief Helper function to trim whitespace from ends of a string.
     * @param str The string to be trimmed.
     * @return A new string without leading or trailing whitespaces.
     * @note Time complexity: O(L), where L is the length of the string.
     */
    std::string trim(const std::string& str) const;

    /**
     * @brief Helper function to split a string by a given character delimiter.
     * @param str The string to be split.
     * @param delimiter The character used as a split boundary.
     * @return A vector of trimmed substring tokens.
     * @note Time complexity: O(L), where L is the length of the string.
     */
    std::vector<std::string> split(const std::string& str, char delimiter) const;

    /**
     * @brief Parses a line of submission data and adds it to the list.
     * @param tokens Vector of string tokens extracted from the CSV line.
     * @note Time complexity: O(1) assuming a bounded number of tokens per line.
     */
    void parseSubmission(const std::vector<std::string>& tokens);

    /**
     * @brief Parses a line of reviewer data and adds it to the list.
     * @param tokens Vector of string tokens extracted from the CSV line.
     * @note Time complexity: O(1) assuming a bounded number of tokens per line.
     */
    void parseReviewer(const std::vector<std::string>& tokens);

    /**
     * @brief Parses a key-value parameter and stores it in the parameters map.
     * @param tokens Vector containing the key and the value.
     * @note Time complexity: O(log P), where P is the number of stored parameters (due to std::map insertion).
     */
    void parseParameter(const std::vector<std::string>& tokens);

    /**
     * @brief Parses a control directive and stores it in the control data vector.
     * @param tokens Vector containing the control variables.
     * @note Time complexity: O(1) amortized for vector push_back.
     */
    void parseControl(const std::vector<std::string>& tokens);

public:
    /**
     * @brief Default constructor.
     */
    DataLoader() = default;

    /**
     * @brief Loads and parses data from the specified setup CSV file.
     * * Reads the file line by line, identifies the current section via '#' headers,
     * splits the lines by commas, and delegates the creation of objects to the 
     * specific parsing helper functions.
     * * @param filename Path to the configuration CSV file.
     * @return true if successful, false if the file cannot be opened.
     * @note Time complexity: O(N * L), where N is the number of lines in the file and L is the maximum line length.
     */
    bool loadData(const std::string& filename);

    /**
     * @brief Retrieves the loaded submissions.
     * @return Constant reference to the vector of submissions.
     * @note Time complexity: O(1).
     */
    const std::vector<Submission>& getSubmissions() const;

    /**
     * @brief Retrieves the loaded reviewers.
     * @return Constant reference to the vector of reviewers.
     * @note Time complexity: O(1).
     */
    const std::vector<Reviewer>& getReviewers() const;

    /**
     * @brief Retrieves the global parameters.
     * @return Constant reference to the map of parameters.
     * @note Time complexity: O(1).
     */
    const std::map<std::string, std::string>& getParameters() const;

    /**
     * @brief Retrieves the control data configurations.
     * @return Constant reference to a vector of tokenized control data lines.
     * @note Time complexity: O(1).
     */
    const std::vector<std::vector<std::string>>& getControlData() const;
    
    /**
     * @brief Gets the configured minimum reviews required per submission.
     * @return The minimum number of reviews, or 0 if not defined.
     * @note Time complexity: O(log P), where P is the number of parameters (map lookup).
     */
    int getMinReviewsPerSubmission() const;

    /**
     * @brief Gets the configured maximum reviews a reviewer can handle.
     * @return The maximum number of reviews, or 0 if not defined.
     * @note Time complexity: O(log P), where P is the number of parameters.
     */
    int getMaxReviewsPerReviewer() const;

    /**
     * @brief Gets the desired output filename specified in the control section.
     * @return The specified filename as a string, or "output.csv" as default.
     * @note Time complexity: O(C), where C is the number of control data entries.
     */
    std::string getOutputFileName() const;

    /**
     * @brief Gets the target reviewer ID for Risk Analysis (if specified).
     * @return The requested risk analysis mode/ID, or 0 if not requested.
     * @note Time complexity: O(C), where C is the number of control data entries.
     */
    int getRiskAnalysis() const;

    /**
     * @brief Checks the requested assignment generation mode.
     * @return Integer code representing the mode, or 0 if not found.
     * @note Time complexity: O(C), where C is the number of control data entries.
     */
    int getGenerateAssignments() const;
};

#endif // DATALOADER_H