#include "Submission.h"

// Default constructor
Submission::Submission() 
    : id(0), title(""), authors(""), email(""), primaryDomain(0), secondaryDomain(0) {}

// Parameterized constructor
Submission::Submission(int id, const std::string& title, const std::string& authors, const std::string& email, int primaryDomain, int secondaryDomain)
    : id(id), title(title), authors(authors), email(email), primaryDomain(primaryDomain), secondaryDomain(secondaryDomain) {}

// Getters
int Submission::getId() const {
    return id;
}

const std::string& Submission::getTitle() const {
    return title;
}

const std::string& Submission::getAuthors() const {
    return authors;
}

const std::string& Submission::getEmail() const {
    return email;
}

int Submission::getPrimaryDomain() const {
    return primaryDomain;
}

int Submission::getSecondaryDomain() const {
    return secondaryDomain;
}
