#pragma once
#include <optional>
#include <string>
#include <vector>

#include "Booking.h"

class TravelAgency {
    std::vector<Booking*> bookings{};

public:
    TravelAgency() = default;

    std::string readJsonFile(const std::string &name);
    std::string readBinaryFile(const std::string &name);
    void printBookings() const;
    std::vector<Booking*>& getBookings();
};
