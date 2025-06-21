#pragma once
#include <string>
#include <vector>

#include "Travel.h"
#include "../serde/prelude.h"

class Customer : public std::enable_shared_from_this<Customer> {
    long id;
    std::string lastName;
    std::string firstName;
    std::vector<std::shared_ptr<Travel>> travelList{};

public:
    Customer(const long id, const std::string &lastName, const std::string &firstName)
        : id(id),
          lastName(lastName),
          firstName(firstName) {
    }

    long getId() const;

    void addTravel(std::shared_ptr<Travel> travel);

    std::vector<std::shared_ptr<Travel>> getTravels();

    std::string &getLastName();

    std::string &getFirstName();

    void serializeAll(nlohmann::json &json);
};

template<>
struct serde_objects::Codec<std::shared_ptr<Customer>> {
    static void serialize(std::shared_ptr<Customer> &obj, serde::Encoder *encoder);

    static std::shared_ptr<Customer> deserialize(serde::Decoder *decoder);
};
