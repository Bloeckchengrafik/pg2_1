#include "Customer.h"

long Customer::getId() const {
    return id;
}

void Customer::addTravel(std::shared_ptr<Travel> travel) {
    if (std::ranges::find(travelList, travel) != travelList.end()) return;
    travelList.push_back(travel);
}

std::string &Customer::getLastName() {
    return lastName;
}

std::string &Customer::getFirstName() {
    return firstName;
}

std::vector<std::shared_ptr<Travel>> Customer::getTravels() {
    return travelList;
}

void Customer::serializeAll(nlohmann::json &json) {
    auto encoder = new serde::json::JsonEncoder();
    auto self = shared_from_this();
    serde_objects::Codec<std::shared_ptr<Customer>>::serialize(self, encoder);

    for (auto travel: travelList) {
        auto travelEncoder = encoder->clone();
        travel->serializeAll(json, travelEncoder);
        delete travelEncoder;
    }

    delete encoder;
}

void serde_objects::Codec<std::shared_ptr<Customer>>::serialize(std::shared_ptr<Customer> &obj, serde::Encoder *encoder) {
    encoder->encode<const long>("customerId", obj->getId())
            .encode<const std::string>("customerFirstName", obj->getFirstName())
            .encode<const std::string>("customerLastName", obj->getLastName());
}

std::shared_ptr<Customer> serde_objects::Codec<std::shared_ptr<Customer>>::deserialize(serde::Decoder *decoder) {
    return std::make_shared<Customer>(
        decoder->at<const long>("customerId"),
        decoder->at<const std::string>("customerFirstName"),
        decoder->at<const std::string>("customerLastName")
    );
}
