#ifndef REVIEWER_H
#define REVIEWER_H

#include <string>

/**
 * @brief Represents a reviewer for scientific articles.
 * 
 * This class stores the details of a reviewer, including their unique ID, 
 * name, contact email, and their primary and secondary areas of expertise.
 */
class Reviewer {
private:
    int id;                     ///< Unique identifier for the reviewer
    std::string name;           ///< Name of the reviewer
    std::string email;          ///< Contact email of the reviewer
    int primaryExpertise;       ///< Primary area of expertise (represented as an integer ID)
    int secondaryExpertise;     ///< Secondary area of expertise (represented as an integer ID)

public:
    /**
     * @brief Default constructor.
     * Initializes an empty reviewer with default values.
     */
    Reviewer();

    /**
     * @brief Parameterized constructor.
     * 
     * @param id The unique identifier of the reviewer.
     * @param name The name of the reviewer.
     * @param email The contact email.
     * @param primaryExpertise The primary expertise ID.
     * @param secondaryExpertise The secondary expertise ID.
     */
    Reviewer(int id, const std::string& name, const std::string& email, int primaryExpertise, int secondaryExpertise);

    /**
     * @brief Gets the reviewer ID.
     * @return The unique ID of the reviewer.
     */
    int getId() const;

    /**
     * @brief Gets the reviewer name.
     * @return The name of the reviewer.
     */
    const std::string& getName() const;

    /**
     * @brief Gets the contact email.
     * @return The contact email.
     */
    const std::string& getEmail() const;

    /**
     * @brief Gets the primary expertise.
     * @return The primary expertise ID.
     */
    int getPrimaryExpertise() const;

    /**
     * @brief Gets the secondary expertise.
     * @return The secondary expertise ID.
     */
    int getSecondaryExpertise() const;
};

#endif // REVIEWER_H
