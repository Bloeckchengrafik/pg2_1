#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"
#include "../coord/Position.h"

typedef Position<"pickupLatitude", "pickupLongitude"> PickupPosition;
typedef Position<"returnLatitude", "returnLongitude"> ReturnPosition;

class RentalCarReservation final : public Booking {
    friend class RentalCarUi;
    std::string pickupLocation;
    std::string returnLocation;
    std::string company;
    PickupPosition pickupPosition;
    ReturnPosition returnPosition;

public:
    RentalCarReservation(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string pickup_location,
        std::string return_location,
        std::string company,
        PickupPosition pickupPosition,
        ReturnPosition returnPosition
    );

    std::string showDetails() override;

    void showEditor(ChangeController *changeController) override;

    std::string &getPickupLocation();

    std::string &getReturnLocation();

    std::string &getCompany();

    PickupPosition &getPickupPosition();

    ReturnPosition &getReturnPosition();

    void setPickupLocation(std::string pickupLocation);
    void setReturnLocation(std::string returnLocation);
    void setCompany(std::string company);
    void setPickupPosition(PickupPosition pickupPosition);
    void setReturnPosition(ReturnPosition returnPosition);

    QIcon getIcon() override;
};

template<> struct serde_objects::Codec<RentalCarReservation*> {
    static void serialize(RentalCarReservation* &obj, serde::Encoder *encoder);
    static RentalCarReservation* deserialize(serde::Decoder *decoder);
};
