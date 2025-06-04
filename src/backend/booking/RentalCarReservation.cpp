#include "RentalCarReservation.h"
#include "../../ui/rentalcarreservationui.h"

RentalCarReservation::RentalCarReservation(
    const std::string &id, const double price, const std::string &fromDate,
    const std::string &toDate, std::string pickup_location,
    std::string return_location,
    std::string company
): Booking(id, price, fromDate, toDate),
   pickupLocation(std::move(pickup_location)),
   returnLocation(std::move(return_location)),
   company(std::move(company)) {
}

std::string &RentalCarReservation::getPickupLocation() {
    return pickupLocation;
}

std::string &RentalCarReservation::getReturnLocation() {
    return returnLocation;
}

std::string &RentalCarReservation::getCompany() {
    return company;
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

void serde_objects::Codec<RentalCarReservation *>::serialize(RentalCarReservation *&obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("pickupLocation", obj->getPickupLocation())
            .encode<const std::string>("returnLocation", obj->getReturnLocation())
            .encode<const std::string>("company", obj->getCompany());
}

RentalCarReservation *serde_objects::Codec<RentalCarReservation *>::deserialize(serde::Decoder *decoder) {
    return new RentalCarReservation(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("pickupLocation", {serde::validate::assertNotEmpty("Pickup location cannot be empty")}),
        decoder->at<std::string>("returnLocation", {serde::validate::assertNotEmpty("Return location cannot be empty")}),
        decoder->at<std::string>("company", {serde::validate::assertNotEmpty("Company cannot be empty")})
    );
}
