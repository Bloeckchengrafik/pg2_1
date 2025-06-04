#pragma once
#include <string>
#include <vector>

#include "Travel.h"

class Customer {
    long id;
    std::string lastName;
    std::string firstName;
    std::vector<Travel *> travelList{};

public:
    Customer(const long id, const std::string &lastName, const std::string &firstName)
        : id(id),
          lastName(lastName),
          firstName(firstName) {
    }

    long getId() const;

    void addTravel(Travel *travel);
};

