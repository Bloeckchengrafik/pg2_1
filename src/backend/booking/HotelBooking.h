#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"
#include "../coord/Position.h"

enum RoomType {
    SINGLE_ROOM,
    DOUBLE_ROOM,
    SUITE,
    APPARTMENT
};

template<> struct serde_objects::Codec<RoomType> {
    static void serialize(RoomType &obj, serde::Encoder *encoder);
    static RoomType deserialize(serde::Decoder *decoder);
};

typedef Position<"hotelLatitude", "hotelLongitude"> HotelPosition;

class HotelBooking final : public Booking {
    friend class HotelBookingUi;

    std::string hotel;
    std::string town;
    RoomType roomType;
    HotelPosition position;

public:
    HotelBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string hotel,
        std::string town,
        RoomType roomType,
        HotelPosition position
    );

    std::string showDetails() override;

    void showEditor(std::shared_ptr<BookingController> changeController) override;

    std::string &getHotel();

    std::string &getTown();

    HotelPosition &getPosition();

    QIcon getIcon() override;

    RoomType & getRoomType();

    void setHotel(std::string hotel);
    void setRoomType(RoomType roomType);
    void setTown(std::string town);
};

template<> struct serde_objects::Codec<std::shared_ptr<HotelBooking>> {
    static void serialize(std::shared_ptr<HotelBooking> &obj, serde::Encoder *encoder);
    static std::shared_ptr<HotelBooking> deserialize(serde::Decoder *decoder);
};