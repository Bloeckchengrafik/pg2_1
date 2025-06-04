#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

class RentalCarReservation final : public Booking {
    friend class RentalCarUi;
    std::string pickupLocation;
    std::string returnLocation;
    std::string company;

public:
    RentalCarReservation(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string pickup_location,
        std::string return_location,
        std::string company
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getPickupLocation();

    std::string &getReturnLocation();

    std::string &getCompany();
};

template<> struct serde_objects::Codec<RentalCarReservation*> {
    static void serialize(RentalCarReservation* &obj, serde::Encoder *encoder);
    static RentalCarReservation* deserialize(serde::Decoder *decoder);
};
