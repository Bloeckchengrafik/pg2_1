#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"
#include "../coord/Position.h"

enum BookingClass {
    ECONOMY,
    PREMIUM_ECONOMY,
    BUSINESS,
    FIRST_CLASS
};

template<>
struct serde_objects::Codec<BookingClass> {
    static void serialize(BookingClass &obj, serde::Encoder *encoder);

    static BookingClass deserialize(serde::Decoder *decoder);
};

typedef Position<"fromDestLatitude", "fromDestLongitude"> FromDestPosition;
typedef Position<"toDestLatitude", "toDestLongitude"> ToDestPosition;

class FlightBooking final : public Booking {
    friend class FlightBookingUi;

    std::string fromDestination;
    FromDestPosition fromDestinationPosition;
    std::string toDestination;
    ToDestPosition toDestinationPosition;
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
        BookingClass bookingClass,
        FromDestPosition fromDestinationPosition,
        ToDestPosition toDestinationPosition
    );

    std::string showDetails() override;

    void showEditor(std::shared_ptr<BookingController> changeController) override;

    std::string &getFromDestination();

    std::string &getToDestination();

    std::string &getAirline();

    FromDestPosition &getFromDestPosition();
    ToDestPosition &getToDestPosition();

    void setAirline(std::string airline);

    void setBookingClass(BookingClass bookingClass);

    void setFromDestination(std::string fromDestination);

    void setToDestination(std::string toDestination);

    QIcon getIcon() override;

    BookingClass &getBookingClass();
};

template<>
struct serde_objects::Codec<std::shared_ptr<FlightBooking>> {
    static void serialize(std::shared_ptr<FlightBooking> &obj, serde::Encoder *encoder);

    static std::shared_ptr<FlightBooking> deserialize(serde::Decoder *decoder);
};
