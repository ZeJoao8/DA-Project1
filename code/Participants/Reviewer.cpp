#include "Reviewer.h"

// Default constructor
Reviewer::Reviewer() 
    : id(0), name(""), email(""), primaryExpertise(0), secondaryExpertise(0) {}

// Parameterized constructor
Reviewer::Reviewer(int id, const std::string& name, const std::string& email, int primaryExpertise, int secondaryExpertise)
    : id(id), name(name), email(email), primaryExpertise(primaryExpertise), secondaryExpertise(secondaryExpertise) {}

// Getters
int Reviewer::getId() const {
    return id;
}

const std::string& Reviewer::getName() const {
    return name;
}

const std::string& Reviewer::getEmail() const {
    return email;
}

int Reviewer::getPrimaryExpertise() const {
    return primaryExpertise;
}

int Reviewer::getSecondaryExpertise() const {
    return secondaryExpertise;
}
