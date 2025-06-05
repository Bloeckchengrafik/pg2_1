#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

enum BookingClass {
    ECONOMY,
    PREMIUM_ECONOMY,
    BUSINESS,
    FIRST_CLASS
};

template<> struct serde_objects::Codec<BookingClass> {
    static void serialize(BookingClass &obj, serde::Encoder *encoder);
    static BookingClass deserialize(serde::Decoder *decoder);
};

class FlightBooking final : public Booking {
    friend class FlightBookingUi;

    std::string fromDestination;
    std::string toDestination;
    std::string airline;
    BookingClass bookingClass;

public:
    FlightBooking(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string fromDestination,
        std::string toDestination,
        std::string airline,
        BookingClass bookingClass
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getFromDestination();

    std::string &getToDestination();

    std::string &getAirline();

    QIcon getIcon() override;

    BookingClass & getBookingClass();
};

template<> struct serde_objects::Codec<FlightBooking*> {
    static void serialize(FlightBooking* &obj, serde::Encoder *encoder);
    static FlightBooking* deserialize(serde::Decoder *decoder);
};
