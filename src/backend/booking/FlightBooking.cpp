//
// Created by chris on 6/4/25.
//

#include "FlightBooking.h"
#include "../../ui/flightbookingui.h"

void serde_objects::Codec<BookingClass>::serialize(BookingClass &obj, serde::Encoder *encoder) {
    switch (obj) {
        case ECONOMY:
            encoder->encodeString("Y");
            break;
        case PREMIUM_ECONOMY:
            encoder->encodeString("W");
            break;
        case BUSINESS:
            encoder->encodeString("J");
            break;
        case FIRST_CLASS:
            encoder->encodeString("F");
            break;
    }
}

BookingClass serde_objects::Codec<BookingClass>::deserialize(serde::Decoder *decoder) {
    const auto str = decoder->decodeString();
    if (str == "Y") return ECONOMY;
    if (str == "W") return PREMIUM_ECONOMY;
    if (str == "J") return BUSINESS;
    if (str == "B") return BUSINESS;
    if (str == "F") return FIRST_CLASS;
    throw serde::ValidationException("Invalid booking class code");
}

FlightBooking::FlightBooking(
    const std::string &id, const double price, const std::string &fromDate,
    const std::string &toDate, std::string fromDestination, std::string toDestination,
    std::string airline, BookingClass bookingClass,
    FromDestPosition from,
    ToDestPosition to
)
    : Booking(id, price, fromDate, toDate),
      fromDestination(std::move(fromDestination)), fromDestinationPosition(from),
      toDestination(std::move(toDestination)), toDestinationPosition(to),
      airline(std::move(airline)),
      bookingClass(bookingClass) {
}

std::string &FlightBooking::getFromDestination() {
    return fromDestination;
}

std::string &FlightBooking::getToDestination() {
    return toDestination;
}

std::string &FlightBooking::getAirline() {
    return airline;
}

FromDestPosition &FlightBooking::getFromDestPosition() {
    return fromDestinationPosition;
}

ToDestPosition &FlightBooking::getToDestPosition() {
    return toDestinationPosition;
}

void FlightBooking::setAirline(std::string airline) {
    this->airline = std::move(airline);
}

void FlightBooking::setBookingClass(BookingClass bookingClass) {
    this->bookingClass = bookingClass;
}

void FlightBooking::setFromDestination(std::string fromDestination) {
    this->fromDestination = std::move(fromDestination);
}

void FlightBooking::setToDestination(std::string toDestination) {
    this->toDestination = std::move(toDestination);
}

QIcon FlightBooking::getIcon() {
    return QIcon(":icons/airplane-in-flight.svg");
}

BookingClass &FlightBooking::getBookingClass() {
    return bookingClass;
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

void FlightBooking::showEditor(ChangeController *changeController) {
    (new FlightBookingUi(this, changeController))->show();
}

void serde_objects::Codec<FlightBooking *>::serialize(FlightBooking *&obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("fromDest", obj->getFromDestination())
            .encode<const std::string>("toDest", obj->getToDestination())
            .encode<const std::string>("airline", obj->getAirline())
            .encode<BookingClass>("bookingClass", obj->getBookingClass());

    obj->getFromDestPosition().serialize(encoder);
    obj->getToDestPosition().serialize(encoder);
}

FlightBooking *serde_objects::Codec<FlightBooking *>::deserialize(serde::Decoder *decoder) {
    return new FlightBooking(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("fromDest", {
                                     serde::validate::assertNotEmpty("From destination cannot be empty"),
                                     serde::validate::assertLength(3, "From destination must be 3 characters")
                                 }),
        decoder->at<std::string>("toDest", {
                                     serde::validate::assertNotEmpty("To destination cannot be empty"),
                                     serde::validate::assertLength(3, "To destination must be 3 characters")
                                 }),
        decoder->at<std::string>("airline", {serde::validate::assertNotEmpty("Airline cannot be empty")}),
        decoder->at<BookingClass>("bookingClass"),
        FromDestPosition::deserialize(decoder),
        ToDestPosition::deserialize(decoder)
    );
}
