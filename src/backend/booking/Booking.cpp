#include "Booking.h"

#include <qdatetime.h>


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

double Booking::getPrice() {
    return this->price;
}

std::string &Booking::getId() {
    return id;
}

std::string &Booking::getFromDate() {
    return fromDate;
}

std::string &Booking::getToDate() {
    return toDate;
}

void Booking::setPrice(double price) {
    this->price = price;
}

void Booking::setFromDate(std::string fromDate) {
    this->fromDate = std::move(fromDate);
}

void Booking::setToDate(std::string toDate) {
    this->toDate = std::move(toDate);
}

