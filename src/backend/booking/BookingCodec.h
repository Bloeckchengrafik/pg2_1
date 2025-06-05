#pragma once
#include "Booking.h"
#include "FlightBooking.h"
#include "HotelBooking.h"
#include "RentalCarReservation.h"
#include "TrainTicket.h"
#include "../../serde/prelude.h"

template<> struct serde_objects::Codec<Booking*> {
    static void serialize(Booking* &obj, serde::Encoder *encoder);
    static Booking* deserialize(serde::Decoder *decoder);
};

void serializeBooking(Booking *booking, serde::Encoder *encoder, nlohmann::json &json);