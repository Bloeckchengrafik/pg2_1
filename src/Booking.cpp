#include "Booking.h"
#include "easyKey.h"
#include <sstream>

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

double Booking::getPrice() const {
    return this->price;
}

std::string RentalCarReservation::showDetails() {
    std::stringstream out;
    out
            << Terminal::Color::CYAN << "Mietwagenreservierung mit " << Terminal::RESET 
            << Terminal::Color::YELLOW << this->company << Terminal::RESET
            << ". Abholung am " << Terminal::Color::GREEN << formatDate(this->fromDate) << Terminal::RESET
            << " in " << Terminal::Color::BLUE << this->pickupLocation << Terminal::RESET << ". "
            << "Rückgabe am " << Terminal::Color::GREEN << formatDate(this->toDate) << Terminal::RESET
            << " in " << Terminal::Color::BLUE << this->returnLocation << Terminal::RESET << ". "
            << "Preis: " << Terminal::Color::MAGENTA << this->price << Terminal::RESET << " Euro";

    return out.str();
}

std::string HotelBooking::showDetails() {
    std::stringstream out;
    out
            << Terminal::Color::CYAN << "Hotelreservierung im " << Terminal::RESET
            << Terminal::Color::YELLOW << this->hotel << Terminal::RESET
            << " in " << Terminal::Color::BLUE << this->town << Terminal::RESET
            << " vom " << Terminal::Color::GREEN << formatDate(this->fromDate) << Terminal::RESET
            << " bis zum " << Terminal::Color::GREEN << formatDate(this->toDate) << Terminal::RESET
            << ". Preis: " << Terminal::Color::MAGENTA << this->price << Terminal::RESET << " Euro";

    return out.str();
}

std::string FlightBooking::showDetails() {
    std::stringstream out;
    out
            << Terminal::Color::CYAN << "Flugbuchung von " << Terminal::RESET
            << Terminal::Color::BLUE << this->fromDestination << Terminal::RESET
            << Terminal::Color::CYAN << " nach " << Terminal::RESET
            << Terminal::Color::BLUE << this->toDestination << Terminal::RESET
            << " mit " << Terminal::Color::YELLOW << this->airline << Terminal::RESET
            << " am " << Terminal::Color::GREEN << formatDate(this->fromDate) << Terminal::RESET
            << ". Preis: " << Terminal::Color::MAGENTA << this->price << Terminal::RESET << " Euro";

    return out.str();
}

std::string TrainTicket::showDetails() {
    std::stringstream out;
    out << Terminal::Color::CYAN << "Zugticket von " << Terminal::RESET
            << Terminal::Color::BLUE << this->fromStation << Terminal::RESET
            << Terminal::Color::CYAN << " nach " << Terminal::RESET
            << Terminal::Color::BLUE << this->toStation << Terminal::RESET
            << " am " << Terminal::Color::GREEN << formatDate(this->fromDate) << Terminal::RESET
            << " von " << Terminal::Color::YELLOW << this->departureTime << Terminal::RESET
            << " bis " << Terminal::Color::YELLOW << this->arrivalTime << Terminal::RESET;

    if (!this->connectingStations.empty()) {
        out << Terminal::Color::CYAN << " über " << Terminal::RESET;
    
        for (auto station = this->connectingStations.begin(); station != this->connectingStations.end(); ++station) {
            out << Terminal::Color::RED << *station << Terminal::RESET;
            if (station != this->connectingStations.end() - 1) {
                out << ", ";
            }
        }
    }
    
    out << ". Preis: " << Terminal::Color::MAGENTA << this->price << Terminal::RESET << " Euro";

    return out.str();
}
