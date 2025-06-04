#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

class HotelBooking final : public Booking {
    friend class HotelBookingUi;

    std::string hotel;
    std::string town;

public:
    HotelBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string hotel,
        std::string town
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getHotel();

    std::string &getTown();
};

template<> struct serde_objects::Codec<HotelBooking*> {
    static void serialize(HotelBooking* &obj, serde::Encoder *encoder);
    static HotelBooking* deserialize(serde::Decoder *decoder);
};