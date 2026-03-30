#ifndef MATCH_H
#define MATCH_H

/**
 * @file Match.h
 * @brief Defines the Match structure for review assignments.
 */

/**
 * @brief Represents a successful assignment between a reviewer and a submission.
 * * This structure stores the details of a match, including the reviewer ID, 
 * the submission ID, and the specific domain that linked them together.
 */
struct Match {
    int revId; ///< ID of the assigned reviewer
    int subId; ///< ID of the assigned submission
    int match; ///< The domain ID that validated this match (e.g., primary or secondary domain)
};

#endif // MATCH_H