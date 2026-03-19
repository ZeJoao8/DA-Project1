#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>

/**
 * @brief Represents a scientific article submission.
 * 
 * This class stores the details of a submission, including its unique ID, 
 * title, authors, contact email, and its primary and secondary research domains.
 */
struct Submission {
    int id;                     ///< Unique identifier for the submission
    std::string title;          ///< Title of the submission
    std::string authors;        ///< Authors of the submission
    std::string email;          ///< Contact email for the submission
    int primaryDomain;          ///< Primary research domain (represented as an integer ID)
    int secondaryDomain;        ///< Secondary research domain (represented as an integer ID)


};

#endif // SUBMISSION_H
