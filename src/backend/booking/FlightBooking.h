#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

class FlightBooking final : public Booking {
    friend class FlightBookingUi;

    std::string fromDestination;
    std::string toDestination;
    std::string airline;

public:
    FlightBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string fromDestination,
        std::string toDestination,
        std::string airline
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getFromDestination();

    std::string &getToDestination();

    std::string &getAirline();
};

template<> struct serde_objects::Codec<FlightBooking*> {
    static void serialize(FlightBooking* &obj, serde::Encoder *encoder);
    static FlightBooking* deserialize(serde::Decoder *decoder);
};
