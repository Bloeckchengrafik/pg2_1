#pragma once
#include "Booking.h"
#include "FlightBooking.h"
#include "HotelBooking.h"
#include "RentalCarReservation.h"
#include "TrainTicket.h"
#include "../../serde/prelude.h"

template<> struct serde_objects::Codec<std::shared_ptr<Booking>> {
    static void serialize(std::shared_ptr<Booking> &obj, serde::Encoder *encoder);
    static std::shared_ptr<Booking> deserialize(serde::Decoder *decoder);
};

void serializeBooking(std::shared_ptr<Booking> booking, serde::Encoder *encoder, nlohmann::json &json);