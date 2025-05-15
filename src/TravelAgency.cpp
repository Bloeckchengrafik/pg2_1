#include "TravelAgency.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <nlohmann/json.hpp>

#include "BinaryFormatReader.h"

using json = nlohmann::json;

std::string assertNotEmpty(const std::string &str, const std::string &message) {
    if (str.empty()) {
        throw std::runtime_error(message);
    }

    return str;
}

std::string assertLength(const std::string &str, const int length, const std::string &message) {
    if (str.length() != length) {
        throw std::runtime_error(message);
    }

    return str;
}

double assertNotNan(const double d, const std::string &message) {
    if (std::isnan(d)) {
        throw std::runtime_error(message);
    }

    return d;
}

void printMetadata(const std::vector<Booking *> &bookings) {
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

    std::cout << "Es wurden " << countFlight << " Flugbuchungen im Wert von " << priceFlight << "€, "
            << countRentalCar << " Mietwagenbuchungen im Wert von " << priceRentalCar << "€, "
            << countHotel << " Hotelreservierungen im Wert von " << priceHotel << "€ und "
            << countTrain << " Zugbuchungen im Wert von " << priceTrain << "€, angelegt" << std::endl;
}

TravelAgency::~TravelAgency() {
    for (Booking* booking : bookings) {
        delete booking;
    }
    bookings.clear();
}

std::optional<std::string> TravelAgency::readJsonFile(const std::string &name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        throw std::runtime_error("file not found");
    }
    std::vector<Booking *> bookings;
    int objCount = 0;
    for (const json data = json::parse(file); json obj: data) {
        objCount++;
        try {
            const std::string type = assertNotEmpty(obj["type"], "Type cannot be empty");

            const std::string id = assertNotEmpty(obj["id"], "ID cannot be empty");
            const double price = assertNotNan(obj["price"], "Price cannot be NaN");
            const std::string fromDate = assertNotEmpty(obj["fromDate"], "From date cannot be empty");
            const std::string toDate = assertNotEmpty(obj["toDate"], "To date cannot be empty");

            if (type == "RentalCar") {
                bookings.push_back(new RentalCarReservation(
                    id,
                    price,
                    fromDate,
                    toDate,
                    assertNotEmpty(obj["pickupLocation"], "Pickup location cannot be empty"),
                    assertNotEmpty(obj["returnLocation"], "Return location cannot be empty"),
                    assertNotEmpty(obj["company"], "Company cannot be empty")
                ));
            } else if (type == "Hotel") {
                bookings.push_back(new HotelBooking(
                    id,
                    price,
                    fromDate,
                    toDate,
                    assertNotEmpty(obj["hotel"], "Hotel cannot be empty"),
                    assertNotEmpty(obj["town"], "Town cannot be empty")
                ));
            } else if (type == "Flight") {
                bookings.push_back(new FlightBooking(
                    id,
                    price,
                    fromDate,
                    toDate,
                    assertLength(assertNotEmpty(obj["fromDest"], "From destination cannot be empty"), 3,
                                 "From destination must be 3 characters"),
                    assertLength(assertNotEmpty(obj["toDest"], "To destination cannot be empty"), 3,
                                 "To destination must be 3 characters"),
                    assertNotEmpty(obj["airline"], "Airline cannot be empty")
                ));
            } else if (type == "Train") {
                const auto &stations = obj["connectingStations"];
                std::vector<std::string> validatedStations;
                for (const auto &station: stations) {
                    validatedStations.push_back(assertNotEmpty(station, "Connecting station cannot be empty"));
                }

                bookings.push_back(new TrainTicket(
                    id,
                    price,
                    fromDate,
                    toDate,
                    assertNotEmpty(obj["arrivalTime"], "Arrival time cannot be empty"),
                    assertNotEmpty(obj["departureTime"], "Departure time cannot be empty"),
                    assertNotEmpty(obj["fromStation"], "From station cannot be empty"),
                    assertNotEmpty(obj["toStation"], "To station cannot be empty"),
                    validatedStations
                ));
            }
        } catch (const std::exception &e) {
            std::stringstream out;
            out << "Error while parsing JSON for object " << objCount << ": " << e.what();
            for (auto &booking: bookings) {
                delete booking;
            }
            return out.str();
        }
    }

    printMetadata(bookings);
    this->bookings.insert(this->bookings.end(), bookings.begin(), bookings.end());

    return std::nullopt;
}

void TravelAgency::readBinaryFile(const std::string &name) {
    BinaryFormatReader formatReader(name);
    std::vector<Booking *> bookings;
    while (!formatReader.isAtEnd()) {
        const char type = formatReader.readChar();
        const auto id = formatReader.readString(38);
        const auto price = formatReader.readDouble();
        const auto fromDate = formatReader.readString(8);
        const auto toDate = formatReader.readString(8);
        if (type == 'F') {
            const auto fromDest = formatReader.readString(3);
            const auto toDest = formatReader.readString(3);
            const auto airline = formatReader.readPaddedString(15);

            bookings.push_back(new FlightBooking(
                id,
                price,
                fromDate,
                toDate,
                fromDest,
                toDest,
                airline
            ));
        } else if (type == 'R') {
            const auto pickupLocation = formatReader.readPaddedString(15);
            const auto returnLocation = formatReader.readPaddedString(15);
            const auto company = formatReader.readPaddedString(15);

            bookings.push_back(new RentalCarReservation(
                id,
                price,
                fromDate,
                toDate,
                pickupLocation,
                returnLocation,
                company
            ));
        } else if (type == 'H') {
            const auto hotel = formatReader.readPaddedString(15);
            const auto town = formatReader.readPaddedString(15);
            bookings.push_back(new HotelBooking(
                id,
                price,
                fromDate,
                toDate,
                hotel,
                town
            ));
        } else if (type == 'T') {
            const auto fromStation = formatReader.readPaddedString(15);
            const auto toStation = formatReader.readPaddedString(15);
            const auto departureTime = formatReader.readString(5);
            const auto arrivalTime = formatReader.readString(5);

            const auto connectingStationCount = formatReader.readInt();
            std::vector<std::string> connectingStations;
            for (int i = 0; i < connectingStationCount; ++i) {
                connectingStations.push_back(formatReader.readPaddedString(15));
            }

            bookings.push_back(new TrainTicket(
                id,
                price,
                fromDate,
                toDate,
                arrivalTime,
                departureTime,
                fromStation,
                toStation,
                connectingStations
            ));
        } else {
            throw std::runtime_error(
                "Unknown booking type: " + std::to_string(type) + " - " + formatReader.getDebugInfo());
        }
    }

    printMetadata(bookings);
    this->bookings.insert(this->bookings.end(), bookings.begin(), bookings.end());
}

void TravelAgency::printBookings() const {
    for (const auto booking: this->bookings) {
        std::cout << booking->showDetails() << std::endl;
    }
}
