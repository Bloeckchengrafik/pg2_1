#include "TravelAgency.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <qfile.h>
#include <nlohmann/json.hpp>

#include "booking/FlightBooking.h"
#include "booking/HotelBooking.h"
#include "booking/RentalCarReservation.h"
#include "booking/TrainTicket.h"
#include "booking/BookingCodec.h"

using json = nlohmann::json;

std::string getMetadata(const std::vector<std::shared_ptr<Booking>> &bookings, const std::vector<std::shared_ptr<Customer>> &customers,
                        const std::vector<std::shared_ptr<Travel>> &travels) {
    double priceFlight = 0;
    double priceRentalCar = 0;
    double priceHotel = 0;
    double priceTrain = 0;

    int countFlight = 0;
    int countRentalCar = 0;
    int countHotel = 0;
    int countTrain = 0;

    for (const auto booking: bookings) {
        if (std::dynamic_pointer_cast<FlightBooking>(booking) != nullptr) {
            countFlight++;
            priceFlight += booking->getPrice();
        } else if (std::dynamic_pointer_cast<RentalCarReservation>(booking) != nullptr) {
            countRentalCar++;
            priceRentalCar += booking->getPrice();
        } else if (std::dynamic_pointer_cast<HotelBooking>(booking) != nullptr) {
            countHotel++;
            priceHotel += booking->getPrice();
        } else if (std::dynamic_pointer_cast<TrainTicket>(booking) != nullptr) {
            countTrain++;
            priceTrain += booking->getPrice();
        }
    }

    std::stringstream out;
    out << "Es wurden " << countFlight << " Flugbuchungen im Wert von " << priceFlight << "€, "
            << countRentalCar << " Mietwagenbuchungen im Wert von " << priceRentalCar << "€, "
            << countHotel << " Hotelreservierungen im Wert von " << priceHotel << "€ und "
            << countTrain << " Zugbuchungen im Wert von " << priceTrain << "€, angelegt" << std::endl;

    out << "Es wurden " << travels.size() << " Reisen und " << customers.size() << " Kunden angelegt" << std::endl;

    out << "Kunde 1 hat " << std::ranges::count_if(travels, [](const std::shared_ptr<Travel> &travel) {
        return travel->getCustomerId() == 1;
    }) << " Reisen gebucht" << std::endl;

    out << "Zur Reise ID 17 gehören " << (*std::ranges::find_if(travels, [](const std::shared_ptr<Travel> &travel) {
        return travel->getId() == 17;
    }))->getBookings().size() << " Buchungen" << std::endl;

    return out.str();
}

void TravelAgency::mergeWith(
    const std::vector<std::shared_ptr<Booking> > &vector,
    const std::vector<std::shared_ptr<Customer> > &customers,
    const std::vector<std::shared_ptr<Travel> > &travels
) {
    for (const auto booking: vector) {
        allBookings.push_back(booking);
    }

    for (auto travel: travels) {
        if (auto found_travel = findTravel(travel->getId()); found_travel == std::nullopt) {
            allTravels.push_back(travel);
        } else {
            for (const auto booking: travel->getBookings()) (*found_travel)->addBooking(booking);
        }
    }

    for (auto customer: customers) {
        if (auto found_customer = findCustomer(customer->getId()); found_customer == std::nullopt) {
            allCustomers.push_back(customer);
        } else {
            for (const auto travel: customer->getTravels()) (*found_customer)->addTravel(travel);
        }
    }
}

TravelAgency::TravelAgency() {
    QFile file(":/iatacodes.json");
    file.open(QIODevice::ReadOnly);
    auto content = file.readAll().toStdString();
    const json contentJson = json::parse(content);
    serde::json::JsonDecoder decoder(contentJson);
    for (const std::shared_ptr airport:
         serde_objects::Codec<std::vector<std::shared_ptr<Airport> > >::deserialize(&decoder)) {
        allAirports[airport->getCode()] = airport;
    }
}

TravelAgency::~TravelAgency() {
    allBookings.clear();
    allCustomers.clear();
    allTravels.clear();
    allAirports.clear();
}

std::string TravelAgency::readFile(const std::string &name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        throw std::runtime_error("file not found");
    }
    std::vector<std::shared_ptr<Booking> > bookings;
    std::vector<std::shared_ptr<Customer> > customers;
    std::vector<std::shared_ptr<Travel> > travels;

    int objCount = 0;
    for (const json data = json::parse(file); json obj: data) {
        objCount++;
        auto decoder = new serde::json::JsonDecoder(obj);
        try {
            auto booking = serde_objects::Codec<std::shared_ptr<Booking>>::deserialize(decoder);
            bookings.push_back(booking);

            auto travelId = decoder->at<const long>("travelId");
            auto travel = std::ranges::find_if(travels, [&travelId](const std::shared_ptr<Travel> travel) {
                return travel->getId() == travelId;
            });

            std::optional<std::shared_ptr<Travel>> travelObj = std::nullopt;

            if (travel == travels.end()) {
                travelObj = serde_objects::Codec<std::shared_ptr<Travel>>::deserialize(decoder);
                travels.push_back(travelObj.value());
            } else {
                travelObj = *travel;
            }

            travelObj.value()->addBooking(booking);

            auto customerId = decoder->at<const long>("customerId");
            auto customer = std::ranges::find_if(customers, [&customerId](const std::shared_ptr<Customer> customer) {
                return customer->getId() == customerId;
            });

            std::optional<std::shared_ptr<Customer>> customerObj = std::nullopt;

            if (customer == customers.end()) {
                customerObj = serde_objects::Codec<std::shared_ptr<Customer>>::deserialize(decoder);
                customers.push_back(customerObj.value());
            } else {
                customerObj = *customer;
            }

            customerObj.value()->addTravel(travelObj.value());
        } catch (const std::exception &e) {
            std::stringstream out;
            out << "Error while parsing JSON for object " << objCount << ": " << e.what();
            throw std::runtime_error(out.str());
        }
        delete decoder;
    }

    printf("found %ld bookings\n", bookings.size());

    const auto metadata = getMetadata(bookings, customers, travels);
    mergeWith(bookings, customers, travels);
    return metadata;
}

void TravelAgency::printBookings() const {
    for (const auto booking: this->allBookings) {
        std::cout << booking->showDetails() << std::endl;
    }
}

std::vector<std::shared_ptr<Booking> > &TravelAgency::getBookings() {
    return this->allBookings;
}

std::optional<std::shared_ptr<Booking> > TravelAgency::findBooking(const std::string &id) {
    const auto el = std::ranges::find_if(
        allBookings,
        [&id](const std::shared_ptr<Booking> &booking) {
            return booking->getId() == id;
        }
    );
    if (el == allBookings.end()) {
        return std::nullopt;
    }
    return *el;
}

std::optional<std::shared_ptr<Customer> > TravelAgency::findCustomer(long id) {
    const auto el = std::ranges::find_if(
        allCustomers,
        [&id](const std::shared_ptr<Customer> &customer) {
            return customer->getId() == id;
        }
    );

    if (el == allCustomers.end()) {
        return std::nullopt;
    }
    return *el;
}

std::optional<std::shared_ptr<Travel> > TravelAgency::findTravel(long id) {
    const auto el = std::ranges::find_if(
        allTravels,
        [&id](const std::shared_ptr<Travel> &travel) {
            return travel->getId() == id;
        }
    );

    if (el == allTravels.end()) {
        return std::nullopt;
    }

    return *el;
}

std::optional<std::shared_ptr<Airport> > TravelAgency::getAirport(const std::string &code) {
    if (const auto it = allAirports.find(code); it != allAirports.end()) {
        return it->second;
    }
    return std::nullopt;
}

void TravelAgency::writeFile(const std::string &fileName) const {
    nlohmann::json data;
    for (auto customer: allCustomers) {
        customer->serializeAll(data);
    }

    std::string jsonData = data.dump(4);
    std::ofstream file(fileName);
    file << jsonData;
}
