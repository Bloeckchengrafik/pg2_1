#pragma once
#include <optional>
#include <string>
#include <vector>

#include "CheckConfiguration.h"
#include "booking/Booking.h"
#include "Customer.h"
#include "coord/Airport.h"

class TravelAgency {
    std::vector<std::shared_ptr<Booking>> allBookings{};
    std::vector<std::shared_ptr<Customer>> allCustomers{};
    std::vector<std::shared_ptr<Travel>> allTravels{};
    std::map<std::string, std::shared_ptr<Airport>> allAirports{};

    std::shared_ptr<CheckConfigurationController> checkConfigController{};

    void mergeWith(const std::vector<std::shared_ptr<Booking>> &vector, const std::vector<std::shared_ptr<Customer>> &customers,
                   const std::vector<std::shared_ptr<Travel>> &travels);

public:
    TravelAgency();

    ~TravelAgency();

    std::string readFile(const std::string &name);

    void printBookings() const;

    std::vector<std::shared_ptr<Booking>> &getBookings();

    std::vector<std::shared_ptr<Customer>>& getCustomers();

    std::optional<std::shared_ptr<Booking>> findBooking(const std::string &id);

    std::optional<std::shared_ptr<Customer>> findCustomer(long id);

    std::optional<std::shared_ptr<Travel>> findTravel(long id);

    std::optional<std::shared_ptr<Airport>> getAirport(const std::string &code);

    void writeFile(const std::string & fileName) const;

    std::shared_ptr<CheckConfigurationController> getCheckConfigController();
};
