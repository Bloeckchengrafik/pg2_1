#include "BookingCodec.h"

void serde_objects::Codec<std::shared_ptr<Booking>>::serialize(std::shared_ptr<Booking> &obj, serde::Encoder *encoder) {
    if (auto rental = std::dynamic_pointer_cast<RentalCarReservation>(obj)) {
        encoder->encode<const std::string>("type", "RentalCar");
        Codec<std::shared_ptr<RentalCarReservation>>::serialize(rental, encoder);
    } else if (auto hotel = std::dynamic_pointer_cast<HotelBooking>(obj)) {
        encoder->encode<const std::string>("type", "Hotel");
        Codec<std::shared_ptr<HotelBooking>>::serialize(hotel, encoder);
    } else if (auto flight = std::dynamic_pointer_cast<FlightBooking>(obj)) {
        encoder->encode<const std::string>("type", "Flight");
        Codec<std::shared_ptr<FlightBooking>>::serialize(flight, encoder);
    } else if (auto train = std::dynamic_pointer_cast<TrainTicket>(obj)) {
        encoder->encode<const std::string>("type", "Train");
        Codec<std::shared_ptr<TrainTicket>>::serialize(train, encoder);
    }
}

std::shared_ptr<Booking> serde_objects::Codec<std::shared_ptr<Booking>>::deserialize(serde::Decoder *decoder) {
    const std::string type = decoder->key("type")->decodeString();
    if (type == "RentalCar") return Codec<std::shared_ptr<RentalCarReservation>>::deserialize(decoder);
    if (type == "Hotel") return Codec<std::shared_ptr<HotelBooking>>::deserialize(decoder);
    if (type == "Flight") return Codec<std::shared_ptr<FlightBooking>>::deserialize(decoder);
    if (type == "Train") return  Codec<std::shared_ptr<TrainTicket>>::deserialize(decoder);
    return nullptr;
}

void serializeBooking(std::shared_ptr<Booking> booking, serde::Encoder *encoder, nlohmann::json &json) {
    serde_objects::Codec<std::shared_ptr<Booking>>::serialize(booking, encoder);
    json.push_back(static_cast<serde::json::JsonEncoder*>(encoder)->getJson());
}
