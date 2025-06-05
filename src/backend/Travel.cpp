//
// Created by chris on 6/4/25.
//

#include "Travel.h"

#include <qdatetime.h>

#include "booking/BookingCodec.h"


long Travel::getId() const {
    return id;
}

long Travel::getCustomerId() const {
    return customerId;
}

void Travel::addBooking(Booking *booking) {
    travelBookings.push_back(booking);
}

std::vector<Booking *> & Travel::getBookings() {
    return travelBookings;
}

bool Travel::operator==(const Travel & other) const {
    return this->id == other.id;
}

std::string Travel::getStart() {
    if (travelBookings.empty()) {
        return "";
    }
    const auto min = std::ranges::min_element(travelBookings.begin(), travelBookings.end(),
                                    [](Booking *a, Booking *b) {
                                        return parseDate(a->getFromDate()) < parseDate(b->getFromDate());
                                    }
    );

    return formatDate((*min)->getFromDate());
}

std::string Travel::getEnd() {
    if (travelBookings.empty()) {
        return "";
    }
    const auto max = std::ranges::max_element(travelBookings.begin(), travelBookings.end(),
                                    [](Booking *a, Booking *b) {
                                        return parseDate(a->getFromDate()) < parseDate(b->getFromDate());
                                    }
    );

    return formatDate((*max)->getToDate());
}

void Travel::serializeAll(nlohmann::json &json, serde::Encoder *encoder) {
    auto self = this;
    serde_objects::Codec<Travel*>::serialize(self, encoder);

    for (auto booking : travelBookings) {
        auto bookingEncoder = encoder->clone();
        serializeBooking(booking, bookingEncoder, json);
        delete bookingEncoder;
    }
}

void serde_objects::Codec<Travel *>::serialize(Travel *&obj, serde::Encoder *encoder) {
    encoder->encode<const long>("travelId", obj->getId())
            .encode<const long>("customerId", obj->getCustomerId());
}

Travel *serde_objects::Codec<Travel *>::deserialize(serde::Decoder *decoder) {
    return new Travel(
        decoder->at<const long>("travelId"),
        decoder->at<const long>("customerId")
    );
}
