#include "Booking.h"

#include <iostream>
#include <qdatetime.h>
#include <sstream>

#include "../ui/flightbookingui.h"
#include "../ui/hotelbookingui.h"
#include "../ui/rentalcarreservationui.h"
#include "../ui/trainticketui.h"

std::string formatDate(const std::string &date) {
    static const std::string months[] = {
        "Jan", "Feb", "Mrz", "Apr", "Mai", "Jun",
        "Jul", "Aug", "Sep", "Okt", "Nov", "Dec"
    };

    const std::string year = date.substr(0, 4);
    const int month = std::stoi(date.substr(4, 2)) - 1;
    const std::string day = date.substr(6, 2);

    return day + ". " + months[month] + " " + year;
}

QDate parseDate(const std::string &date) {
    const int year = std::stoi(date.substr(0, 4));
    const int month = std::stoi(date.substr(4, 2));
    const int day = std::stoi(date.substr(6, 2));

    return {year, month, day};
}

double Booking::getPrice() const {
    return this->price;
}

std::string RentalCarReservation::showDetails() {
    std::stringstream out;
    out
            << "Mietwagenreservierung mit " 
            << this->company
            << ". Abholung am " << formatDate(this->fromDate)
            << " in " << this->pickupLocation << ". "
            << "Rückgabe am " << formatDate(this->toDate)
            << " in " << this->returnLocation << ". "
            << "Preis: " << this->price << " Euro";

    return out.str();
}

void RentalCarReservation::showEditor() {
    const auto ui = new RentalCarReservationUi(this);
    ui->show();
}

std::string HotelBooking::showDetails() {
    std::stringstream out;
    out
            << "Hotelreservierung im "
            << this->hotel
            << " in " << this->town
            << " vom " << formatDate(this->fromDate)
            << " bis zum " << formatDate(this->toDate)
            << ". Preis: " << this->price << " Euro";

    return out.str();
}

void HotelBooking::showEditor() {
    const auto ui = new HotelBookingUi(this);
    ui->show();
}

std::string FlightBooking::showDetails() {
    std::stringstream out;
    out
            << "Flugbuchung von "
            << this->fromDestination
            << " nach "
            << this->toDestination
            << " mit " << this->airline
            << " am " << formatDate(this->fromDate)
            << ". Preis: " << this->price << " Euro";

    return out.str();
}

void FlightBooking::showEditor() {
    (new FlightBookingUi(this))->show();
}

std::string TrainTicket::showDetails() {
    std::stringstream out;
    out << "Zugticket von "
            << this->fromStation
            << " nach "
            << this->toStation
            << " am " << formatDate(this->fromDate)
            << " von " << this->departureTime
            << " bis " << this->arrivalTime;

    if (!this->connectingStations.empty()) {
        out << " über ";
    
        for (auto station = this->connectingStations.begin(); station != this->connectingStations.end(); ++station) {
            out << *station;
            if (station != this->connectingStations.end() - 1) {
                out << ", ";
            }
        }
    }
    
    out << ". Preis: " << this->price << " Euro";

    return out.str();
}

void TrainTicket::showEditor() {
    (new TrainTicketUi(this))->show();
}
