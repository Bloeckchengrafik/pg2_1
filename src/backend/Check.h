#pragma once
#include <memory>

#include "TravelAgency.h"

class CheckError {
    int customer;
    int travelA;
    int travelB;

public:
    CheckError(const int customer, const int travel_a, const int travel_b)
        : customer(customer),
          travelA(travel_a),
          travelB(travel_b) {
    }

    int getCustomer() const {
        return customer;
    }

    int getTravelA() const {
        return travelA;
    }

    int getTravelB() const {
        return travelB;
    }
};

class Check {
    std::shared_ptr<TravelAgency> agency;

public:
    explicit Check(const std::shared_ptr<TravelAgency> &agency);

    std::optional<CheckError> operator()();

private:
    bool checkTravelsDisjunct(std::shared_ptr<Travel> travelA, std::shared_ptr<Travel> travelB);
};
