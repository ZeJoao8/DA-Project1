#ifndef MATCH_H
#define MATCH_H

#include <string>

/**
 * @brief Represents a match.
 * 
 * This class stores the details of a match including the reviewer ID, 
 * the submission ID and matched Domain.
 */
struct Match {
    int revId;                  
    int subId;
    int match;
};

#endif 
