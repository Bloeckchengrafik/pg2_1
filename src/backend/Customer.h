#pragma once
#include <string>
#include <vector>

#include "Travel.h"
#include "../serde/prelude.h"

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

    std::string &getLastName();

    std::string &getFirstName();

    std::vector<Travel *> getTravels();

    void serializeAll(nlohmann::json &json);
};

template<>
struct serde_objects::Codec<Customer *> {
    static void serialize(Customer * &obj, serde::Encoder *encoder);

    static Customer *deserialize(serde::Decoder *decoder);
};
