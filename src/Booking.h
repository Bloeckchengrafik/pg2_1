#pragma once
#include <string>
#include <utility>
#include <vector>

class Booking {
protected:
    std::string id;
    double price;
    std::string fromDate;
    std::string toDate;

public:
    Booking(
        std::string id,
        const double price,
        std::string fromDate,
        std::string toDate
    ) : id(std::move(id)), price(price), fromDate(std::move(fromDate)), toDate(std::move(toDate)) {
    }

    virtual ~Booking() = default;

    virtual std::string showDetails() = 0;

    double getPrice() const;
};

class RentalCarReservation final : public Booking {
    std::string pickupLocation;
    std::string returnLocation;
    std::string company;

public:
    RentalCarReservation(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string pickup_location,
        std::string return_location,
        std::string company
    ) : Booking(id, price, fromDate, toDate),
        pickupLocation(std::move(pickup_location)),
        returnLocation(std::move(return_location)),
        company(std::move(company)) {
    }

    std::string showDetails() override;
};

class HotelBooking final : public Booking {
    std::string hotel;
    std::string town;

public:
    HotelBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string hotel,
        std::string town
    ) : Booking(id, price, fromDate, toDate),
        hotel(std::move(hotel)),
        town(std::move(town)) {
    }

    std::string showDetails() override;
};

class FlightBooking final : public Booking {
    std::string fromDestination;
    std::string toDestination;
    std::string airline;

public:
    FlightBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string fromDestination,
        std::string toDestination,
        std::string airline
    ) : Booking(id, price, fromDate, toDate),
        fromDestination(std::move(fromDestination)),
        toDestination(std::move(toDestination)),
        airline(std::move(airline)) {
    }

    std::string showDetails() override;
};

class TrainTicket final : public Booking {
    std::string fromStation;
    std::string toStation;
    std::string arrivalTime;
    std::string departureTime;
    std::vector<std::string> connectingStations;

public:
    TrainTicket(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string arrivalTime,
        std::string departureTime,
        std::string fromStation,
        std::string toStation,
        const std::vector<std::string> &connectingStations
    ) : Booking(id, price, fromDate, toDate),
        fromStation(std::move(fromStation)),
        toStation(std::move(toStation)),
        arrivalTime(std::move(arrivalTime)),
        departureTime(std::move(departureTime)),
        connectingStations(connectingStations) {
    }

    std::string showDetails() override;
};
