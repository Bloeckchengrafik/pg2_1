#include "RentalCarReservation.h"
#include "../../ui/rentalcarreservationui.h"

RentalCarReservation::RentalCarReservation(
    const std::string &id, const double price, const std::string &fromDate,
    const std::string &toDate, std::string pickup_location,
    std::string return_location,
    std::string company,
    PickupPosition pickupPosition,
    ReturnPosition returnPosition
): Booking(id, price, fromDate, toDate),
   pickupLocation(std::move(pickup_location)),
   returnLocation(std::move(return_location)),
   company(std::move(company)),
   pickupPosition(pickupPosition),
   returnPosition(returnPosition) {
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

void RentalCarReservation::setPickupLocation(std::string pickupLocation) {
    this->pickupLocation = std::move(pickupLocation);
}

void RentalCarReservation::setReturnLocation(std::string returnLocation) {
    this->returnLocation = std::move(returnLocation);
}

void RentalCarReservation::setCompany(std::string company) {
    this->company = std::move(company);
}

PickupPosition &RentalCarReservation::getPickupPosition() {
    return pickupPosition;
}

ReturnPosition &RentalCarReservation::getReturnPosition() {
    return returnPosition;
}

void RentalCarReservation::setPickupPosition(PickupPosition pickupPosition) {
    this->pickupPosition = pickupPosition;
}

void RentalCarReservation::setReturnPosition(ReturnPosition returnPosition) {
    this->returnPosition = returnPosition;
}

QIcon RentalCarReservation::getIcon() {
    return QIcon(":/icons/car-profile.svg");
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

void RentalCarReservation::showEditor(ChangeController *changeController) {
    const auto ui = new RentalCarReservationUi(this, changeController);
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

    obj->getPickupPosition().serialize(encoder);
    obj->getReturnPosition().serialize(encoder);
}

RentalCarReservation *serde_objects::Codec<RentalCarReservation *>::deserialize(serde::Decoder *decoder) {
    return new RentalCarReservation(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("pickupLocation", {serde::validate::assertNotEmpty("Pickup location cannot be empty")}),
        decoder->at<std::string>("returnLocation", {serde::validate::assertNotEmpty("Return location cannot be empty")}),
        decoder->at<std::string>("company", {serde::validate::assertNotEmpty("Company cannot be empty")}),
        PickupPosition::deserialize(decoder),
        ReturnPosition::deserialize(decoder)
    );
}
