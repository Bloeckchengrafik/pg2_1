#pragma once
#include <optional>
#include <string>
#include <vector>

#include "booking/Booking.h"
#include "Customer.h"

class TravelAgency {
    std::vector<Booking*> allBookings{};
    std::vector<Customer*> allCustomers{};
    std::vector<Travel*> allTravels{};

public:
    TravelAgency() = default;
    ~TravelAgency();

    std::string readFile(const std::string &name);
    void printBookings() const;
    std::vector<Booking*>& getBookings();

    std::optional<Booking*> findBooking(const std::string &id);
    std::optional<Customer*> findCustomer(long id);
    std::optional<Travel*> findTravel(long id);
};
