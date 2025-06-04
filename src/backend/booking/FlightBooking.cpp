//
// Created by chris on 6/4/25.
//

#include "FlightBooking.h"
#include "../../ui/flightbookingui.h"

FlightBooking::FlightBooking(const std::string &id, const double price, const std::string &fromDate,
                             const std::string &toDate, std::string fromDestination, std::string toDestination,
                             std::string airline): Booking(id, price, fromDate, toDate),
                                                   fromDestination(std::move(fromDestination)),
                                                   toDestination(std::move(toDestination)),
                                                   airline(std::move(airline)) {
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

void serde_objects::Codec<FlightBooking *>::serialize(FlightBooking *&obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("fromDest", obj->getFromDestination())
            .encode<const std::string>("toDest", obj->getToDestination())
            .encode<const std::string>("airline", obj->getAirline());
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
        decoder->at<std::string>("airline", {serde::validate::assertNotEmpty("Airline cannot be empty")})
    );
}
