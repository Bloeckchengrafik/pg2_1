#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

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

class HotelBooking final : public Booking {
    friend class HotelBookingUi;

    std::string hotel;
    std::string town;
    RoomType roomType;

public:
    HotelBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string hotel,
        std::string town,
        RoomType roomType
    );

    std::string showDetails() override;

    void showEditor(ChangeController *changeController) override;

    std::string &getHotel();

    std::string &getTown();

    QIcon getIcon() override;

    RoomType & getRoomType();

    void setHotel(std::string hotel);
    void setRoomType(RoomType roomType);
    void setTown(std::string town);
};

template<> struct serde_objects::Codec<HotelBooking*> {
    static void serialize(HotelBooking* &obj, serde::Encoder *encoder);
    static HotelBooking* deserialize(serde::Decoder *decoder);
};