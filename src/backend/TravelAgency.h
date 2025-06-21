#pragma once
#include <optional>
#include <string>
#include <vector>

#include "booking/Booking.h"
#include "Customer.h"
#include "coord/Airport.h"

class TravelAgency {
    std::vector<Booking *> allBookings{};
    std::vector<Customer *> allCustomers{};
    std::vector<Travel *> allTravels{};
    std::map<std::string, std::shared_ptr<Airport>> allAirports{};

    void mergeWith(const std::vector<Booking *> &vector, const std::vector<Customer *> &customers,
                   const std::vector<Travel *> &travels);

public:
    TravelAgency();

    ~TravelAgency();

    std::string readFile(const std::string &name);

    void printBookings() const;

    std::vector<Booking *> &getBookings();

    std::optional<Booking *> findBooking(const std::string &id);

    std::optional<Customer *> findCustomer(long id);

    std::optional<Travel *> findTravel(long id);

    void writeFile(const std::string & fileName);
};
