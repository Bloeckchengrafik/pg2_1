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

std::string getMetadata(const std::vector<Booking *> &bookings, const std::vector<Customer *> &customers,
                        const std::vector<Travel *> &travels) {
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
            << countTrain << " Zugbuchungen im Wert von " << priceTrain << "€, angelegt" << std::endl;

    out << "Es wurden " << travels.size() << " Reisen und " << customers.size() << " Kunden angelegt" << std::endl;

    out << "Kunde 1 hat " << std::ranges::count_if(travels, [](const Travel *travel) {
        return travel->getCustomerId() == 1;
    }) << " Reisen gebucht" << std::endl;

    out << "Zur Reise ID 17 gehören " << (*std::ranges::find_if(travels, [](const Travel *travel) {
        return travel->getId() == 17;
    }))->getBookings().size() << " Buchungen" << std::endl;

    return out.str();
}

void TravelAgency::mergeWith(
    const std::vector<Booking *> &vector,
    const std::vector<Customer *> &customers,
    const std::vector<Travel *> &travels
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

    for (const json contentJson = json::parse(content); json element : contentJson.array()) {
        std::cout << contentJson << std::endl;
        serde::json::JsonDecoder decoder(contentJson);
        const auto airport = serde_objects::Codec<std::shared_ptr<Airport>>::deserialize(&decoder);
        allAirports[airport->getCode()] = airport;
    }
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
    allAirports.clear();
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

            auto travelId = decoder->at<const long>("travelId");
            auto travel = std::ranges::find_if(travels, [&travelId](const Travel *travel) {
                return travel->getId() == travelId;
            });

            Travel *travelObj = nullptr;

            if (travel == travels.end()) {
                travelObj = serde_objects::Codec<Travel *>::deserialize(decoder);
                travels.push_back(travelObj);
            } else {
                travelObj = *travel;
            }

            travelObj->addBooking(booking);

            auto customerId = decoder->at<const long>("customerId");
            auto customer = std::ranges::find_if(customers, [&customerId](const Customer *customer) {
                return customer->getId() == customerId;
            });

            Customer *customerObj = nullptr;

            if (customer == customers.end()) {
                customerObj = serde_objects::Codec<Customer *>::deserialize(decoder);
                customers.push_back(customerObj);
            } else {
                customerObj = *customer;
            }

            customerObj->addTravel(travelObj);
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

std::vector<Booking *> &TravelAgency::getBookings() {
    return this->allBookings;
}

std::optional<Booking *> TravelAgency::findBooking(const std::string &id) {
    const auto el = std::ranges::find_if(
        allBookings,
        [&id](Booking *booking) {
            return booking->getId() == id;
        }
    );
    if (el == allBookings.end()) {
        return std::nullopt;
    }
    return *el;
}

std::optional<Customer *> TravelAgency::findCustomer(long id) {
    const auto el = std::ranges::find_if(
        allCustomers,
        [&id](Customer *customer) {
            return customer->getId() == id;
        }
    );

    if (el == allCustomers.end()) {
        return std::nullopt;
    }
    return *el;
}

std::optional<Travel *> TravelAgency::findTravel(long id) {
    const auto el = std::ranges::find_if(
        allTravels,
        [&id](const Travel *travel) {
            return travel->getId() == id;
        }
    );

    if (el == allTravels.end()) {
        return std::nullopt;
    }

    return *el;
}

void TravelAgency::writeFile(const std::string &fileName) {
    nlohmann::json data;
    for (auto customer : allCustomers) {
        customer->serializeAll(data);
    }

    std::string jsonData = data.dump(4);
    std::ofstream file(fileName);
    file << jsonData;
}
