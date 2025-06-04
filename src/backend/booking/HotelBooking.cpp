//
// Created by chris on 6/4/25.
//

#include "HotelBooking.h"

#include "../../ui/hotelbookingui.h"

HotelBooking::HotelBooking(const std::string &id, const double price, const std::string &fromDate,
    const std::string &toDate, std::string hotel, std::string town): Booking(id, price, fromDate, toDate),
                                                                     hotel(std::move(hotel)),
                                                                     town(std::move(town)) {
}

std::string & HotelBooking::getHotel() {
    return hotel;
}

std::string & HotelBooking::getTown() {
    return town;
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

void serde_objects::Codec<HotelBooking*>::serialize(HotelBooking *&obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("hotel", obj->getHotel())
            .encode<const std::string>("town", obj->getTown());
}

HotelBooking * serde_objects::Codec<HotelBooking*>::deserialize(serde::Decoder *decoder) {
    return new HotelBooking(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("hotel", {serde::validate::assertNotEmpty("Hotel cannot be empty")}),
        decoder->at<std::string>("town", {serde::validate::assertNotEmpty("Town cannot be empty")})
    );
}
