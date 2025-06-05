#include "BookingCodec.h"

void serde_objects::Codec<Booking*>::serialize(Booking *&obj, serde::Encoder *encoder) {
    if (auto rental = dynamic_cast<RentalCarReservation*>(obj)) {
        encoder->encode<const std::string>("type", "RentalCar");
        Codec<RentalCarReservation*>::serialize(rental, encoder);
    } else if (auto hotel = dynamic_cast<HotelBooking*>(obj)) {
        encoder->encode<const std::string>("type", "Hotel");
        Codec<HotelBooking*>::serialize(hotel, encoder);
    } else if (auto flight = dynamic_cast<FlightBooking*>(obj)) {
        encoder->encode<const std::string>("type", "Flight");
        Codec<FlightBooking*>::serialize(flight, encoder);
    } else if (auto train = dynamic_cast<TrainTicket*>(obj)) {
        encoder->encode<const std::string>("type", "Train");
        Codec<TrainTicket*>::serialize(train, encoder);
    }
}

Booking * serde_objects::Codec<Booking*>::deserialize(serde::Decoder *decoder) {
    const std::string type = decoder->key("type")->decodeString();
    if (type == "RentalCar") return Codec<RentalCarReservation*>::deserialize(decoder);
    if (type == "Hotel") return Codec<HotelBooking*>::deserialize(decoder);
    if (type == "Flight") return Codec<FlightBooking*>::deserialize(decoder);
    if (type == "Train") return  Codec<TrainTicket*>::deserialize(decoder);
    return nullptr;
}

void serializeBooking(Booking *booking, serde::Encoder *encoder, nlohmann::json &json) {
    serde_objects::Codec<Booking*>::serialize(booking, encoder);
    json.push_back(static_cast<serde::json::JsonEncoder*>(encoder)->getJson());
}
