//
// Created by chris on 6/4/25.
//

#include "HotelBooking.h"

#include "../../ui/hotelbookingui.h"

void serde_objects::Codec<RoomType>::serialize(RoomType &obj, serde::Encoder *encoder) {
    switch (obj) {
        case SINGLE_ROOM:
            encoder->encodeString("EZ");
            break;
        case DOUBLE_ROOM:
            encoder->encodeString("DZ");
            break;
        case SUITE:
            encoder->encodeString("SU");
            break;
        case APPARTMENT:
            encoder->encodeString("AP");
            break;
    }
}

RoomType serde_objects::Codec<RoomType>::deserialize(serde::Decoder *decoder) {
    const auto str = decoder->decodeString();
    if (str == "EZ") return SINGLE_ROOM;
    if (str == "DZ") return DOUBLE_ROOM;
    if (str == "SU") return SUITE;
    if (str == "AP") return APPARTMENT;
    throw serde::ValidationException("Invalid room type code");
}

HotelBooking::HotelBooking(const std::string &id, const double price, const std::string &fromDate,
                           const std::string &toDate, std::string hotel, std::string town,
                           RoomType type, HotelPosition pos)
    : Booking(id, price, fromDate, toDate),
      hotel(std::move(hotel)),
      town(std::move(town)), roomType(type), position(pos) {
}

std::string &HotelBooking::getHotel() {
    return hotel;
}

std::string &HotelBooking::getTown() {
    return town;
}

HotelPosition & HotelBooking::getPosition() {
    return position;
}

QIcon HotelBooking::getIcon() {
    return QIcon(":/icons/bed.svg");
}

RoomType &HotelBooking::getRoomType() {
    return roomType;
}

void HotelBooking::setHotel(std::string hotel) {
    this->hotel = std::move(hotel);
}

void HotelBooking::setTown(std::string town) {
    this->town = std::move(town);
}

void HotelBooking::setRoomType(RoomType roomType) {
    this->roomType = roomType;
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

void HotelBooking::showEditor(std::shared_ptr<BookingController> changeController) {
    (new HotelBookingUi(std::static_pointer_cast<HotelBooking>(shared_from_this()), changeController))->show();
}

void serde_objects::Codec<std::shared_ptr<HotelBooking> >::serialize(std::shared_ptr<HotelBooking> &obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("hotel", obj->getHotel())
            .encode<const std::string>("town", obj->getTown())
            .encode<RoomType>("roomType", obj->getRoomType());

    obj->getPosition().serialize(encoder);
}

std::shared_ptr<HotelBooking> serde_objects::Codec<std::shared_ptr<HotelBooking>>::deserialize(serde::Decoder *decoder) {
    return std::make_shared<HotelBooking>(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("hotel", {serde::validate::assertNotEmpty("Hotel cannot be empty")}),
        decoder->at<std::string>("town", {serde::validate::assertNotEmpty("Town cannot be empty")}),
        decoder->at<RoomType>("roomType"),
        HotelPosition::deserialize(decoder)
    );
}
