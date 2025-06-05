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
