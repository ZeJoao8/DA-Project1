#ifndef MISSING_H
#define MISSING_H

/**
 * @file Missing.h
 * @brief Defines the Missing structure for tracking unmet review requirements.
 */

/**
 * @brief Represents a submission that failed to gather the minimum required reviews.
 * * This structure is used to track and report submissions that were not assigned 
 * enough reviewers during the Maximum Flow execution.
 */
struct Missing {
    int subId;        ///< ID of the submission lacking reviews
    int domain;       ///< Primary domain of the submission
    int missingCount; ///< Number of reviewers still needed to meet the minimum requirement
};

#endif // MISSING_H