#ifndef REVIEWER_H
#define REVIEWER_H

#include <string>

/**
 * @brief Represents a reviewer for scientific articles.
 * 
 * This class stores the details of a reviewer, including their unique ID, 
 * name, contact email, and their primary and secondary areas of expertise.
 */
struct Reviewer {
    int id;                     ///< Unique identifier for the reviewer
    std::string name;           ///< Name of the reviewer
    std::string email;          ///< Contact email of the reviewer
    int primaryExpertise;       ///< Primary area of expertise (represented as an integer ID)
    int secondaryExpertise;     ///< Secondary area of expertise (represented as an integer ID)


};

#endif // REVIEWER_H
