#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>

/**
 * @brief Represents a scientific article submission.
 * 
 * This class stores the details of a submission, including its unique ID, 
 * title, authors, contact email, and its primary and secondary research domains.
 */
class Submission {
private:
    int id;                     ///< Unique identifier for the submission
    std::string title;          ///< Title of the submission
    std::string authors;        ///< Authors of the submission
    std::string email;          ///< Contact email for the submission
    int primaryDomain;          ///< Primary research domain (represented as an integer ID)
    int secondaryDomain;        ///< Secondary research domain (represented as an integer ID)

public:
    /**
     * @brief Default constructor.
     * Initializes an empty submission with default values.
     */
    Submission();

    /**
     * @brief Parameterized constructor.
     * 
     * @param id The unique identifier of the submission.
     * @param title The title of the submission.
     * @param authors The authors of the submission.
     * @param email The contact email.
     * @param primaryDomain The primary domain ID.
     * @param secondaryDomain The secondary domain ID.
     */
    Submission(int id, const std::string& title, const std::string& authors, const std::string& email, int primaryDomain, int secondaryDomain);

    /**
     * @brief Gets the submission ID.
     * @return The unique ID of the submission.
     */
    int getId() const;

    /**
     * @brief Gets the submission title.
     * @return The title of the submission.
     */
    const std::string& getTitle() const;

    /**
     * @brief Gets the authors.
     * @return The authors of the submission.
     */
    const std::string& getAuthors() const;

    /**
     * @brief Gets the contact email.
     * @return The contact email.
     */
    const std::string& getEmail() const;

    /**
     * @brief Gets the primary domain.
     * @return The primary domain ID.
     */
    int getPrimaryDomain() const;

    /**
     * @brief Gets the secondary domain.
     * @return The secondary domain ID.
     */
    int getSecondaryDomain() const;
};

#endif // SUBMISSION_H
