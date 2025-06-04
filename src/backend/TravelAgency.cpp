#include "TravelAgency.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <nlohmann/json.hpp>

#include "booking/FlightBooking.h"
#include "booking/HotelBooking.h"
#include "booking/RentalCarReservation.h"
#include "booking/TrainTicket.h"
#include "booking/BookingCodec.h"

using json = nlohmann::json;

std::string getMetadata(const std::vector<Booking *> &bookings) {
    double priceFlight = 0;
    double priceRentalCar = 0;
    double priceHotel = 0;
    double priceTrain = 0;

    int countFlight = 0;
    int countRentalCar = 0;
    int countHotel = 0;
    int countTrain = 0;

    for (const auto booking: bookings) {
        if (dynamic_cast<FlightBooking *>(booking) != nullptr) {
            countFlight++;
            priceFlight += booking->getPrice();
        } else if (dynamic_cast<RentalCarReservation *>(booking) != nullptr) {
            countRentalCar++;
            priceRentalCar += booking->getPrice();
        } else if (dynamic_cast<HotelBooking *>(booking) != nullptr) {
            countHotel++;
            priceHotel += booking->getPrice();
        } else if (dynamic_cast<TrainTicket *>(booking) != nullptr) {
            countTrain++;
            priceTrain += booking->getPrice();
        }
    }

    std::stringstream out;
    out << "Es wurden " << countFlight << " Flugbuchungen im Wert von " << priceFlight << "€, "
            << countRentalCar << " Mietwagenbuchungen im Wert von " << priceRentalCar << "€, "
            << countHotel << " Hotelreservierungen im Wert von " << priceHotel << "€ und "
            << countTrain << " Zugbuchungen im Wert von " << priceTrain << "€, angelegt";

    return out.str();
}

TravelAgency::~TravelAgency() {
    for (const Booking *booking: allBookings) {
        delete booking;
    }
    for (const Customer *customer: allCustomers) {
        delete customer;
    }
    for (const Travel *travel: allTravels) {
        delete travel;
    }
    allBookings.clear();
    allCustomers.clear();
    allTravels.clear();
}

std::string TravelAgency::readFile(const std::string &name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        throw std::runtime_error("file not found");
    }
    std::vector<Booking *> bookings;
    std::vector<Customer *> customers;
    std::vector<Travel *> travels;

    int objCount = 0;
    for (const json data = json::parse(file); json obj: data) {
        objCount++;
        auto decoder = new serde::json::JsonDecoder(obj);
        try {
            auto booking = serde_objects::Codec<Booking *>::deserialize(decoder);
            bookings.push_back(booking);
        } catch (const std::exception &e) {
            std::stringstream out;
            out << "Error while parsing JSON for object " << objCount << ": " << e.what();
            for (auto &booking: bookings) {
                delete booking;
            }
            throw std::runtime_error(out.str());
        }
        delete decoder;
    }

    const auto metadata = getMetadata(bookings);
    this->allBookings.insert(this->allBookings.end(), bookings.begin(), bookings.end());
    return metadata;
}

void TravelAgency::printBookings() const {
    for (const auto booking: this->allBookings) {
        std::cout << booking->showDetails() << std::endl;
    }
}

std::vector<Booking *> &TravelAgency::getBookings() {
    return this->allBookings;
}

std::optional<Booking *> TravelAgency::findBooking(const std::string &id) {
    return *std::ranges::find_if(
        allBookings,
        [&id](Booking *booking) {
            return booking->getId() == id;
        }
    );
}

std::optional<Customer *> TravelAgency::findCustomer(long id) {
    return *std::ranges::find_if(
        allCustomers,
        [&id](Customer *customer) {
            return customer->getId() == id;
        }
    );
}

std::optional<Travel *> TravelAgency::findTravel(long id) {
    return *std::ranges::find_if(
        allTravels,
        [&id](const Travel *travel) {
            return travel->getId() == id;
        }
    );
}
