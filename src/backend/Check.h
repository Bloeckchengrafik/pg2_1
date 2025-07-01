#pragma once
#include <memory>

#include "TravelAgency.h"

class CheckError {
    int customer;
    std::string message;

public:
    CheckError(const int customer, const std::string &message)
        : customer(customer), message(message) {
    }

    int getCustomer() const {
        return customer;
    }

    std::string getMessage() const {
        return message;
    }
};

class Check {
    std::shared_ptr<TravelAgency> agency;

public:
    explicit Check(const std::shared_ptr<TravelAgency> &agency);

    std::vector<CheckError> operator()();

private:
    bool checkTravelsDisjunct(std::shared_ptr<Travel> travelA, std::shared_ptr<Travel> travelB);
    bool checkNoMissingHotels(std::shared_ptr<Travel> travel);
    bool checkNoOverlappingHotels(std::shared_ptr<Travel> travel);
    bool checkNoOverlappingRentalCars(std::shared_ptr<Travel> travel);
};
