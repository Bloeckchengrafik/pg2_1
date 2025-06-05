#include "Customer.h"

long Customer::getId() const {
    return id;
}

void Customer::addTravel(Travel *travel) {
    if (std::ranges::find(travelList, travel) != travelList.end()) return;
    travelList.push_back(travel);
}

std::string &Customer::getLastName() {
    return lastName;
}

std::string &Customer::getFirstName() {
    return firstName;
}

std::vector<Travel *> Customer::getTravels() {
    return travelList;
}

void Customer::serializeAll(nlohmann::json &json) {
    auto encoder = new serde::json::JsonEncoder();
    auto self = this;
    serde_objects::Codec<Customer *>::serialize(self, encoder);

    for (auto travel: travelList) {
        auto travelEncoder = encoder->clone();
        travel->serializeAll(json, travelEncoder);
        delete travelEncoder;
    }

    delete encoder;
}

void serde_objects::Codec<Customer *>::serialize(Customer *&obj, serde::Encoder *encoder) {
    encoder->encode<const long>("customerId", obj->getId())
            .encode<const std::string>("customerFirstname", obj->getFirstName())
            .encode<const std::string>("customerLastname", obj->getLastName());
}

Customer *serde_objects::Codec<Customer *>::deserialize(serde::Decoder *decoder) {
    return new Customer(
        decoder->at<const long>("customerId"),
        decoder->at<const std::string>("customerFirstname"),
        decoder->at<const std::string>("customerLastname")
    );
}
