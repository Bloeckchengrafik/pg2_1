#pragma once
#include <optional>
#include <string>
#include <vector>

#include "Booking.h"

class TravelAgency {
    std::vector<Booking*> bookings{};

public:
    TravelAgency() = default;
    ~TravelAgency();


    std::optional<std::string> readJsonFile(const std::string &name);
    void readBinaryFile(const std::string &name);
    void printBookings() const;
};
