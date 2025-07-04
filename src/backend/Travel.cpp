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

void Travel::addBooking(std::shared_ptr<Booking> booking) {
    travelBookings.push_back(booking);
}

std::vector<std::shared_ptr<Booking> > &Travel::getBookings() {
    return travelBookings;
}

bool Travel::operator==(const Travel &other) const {
    return this->id == other.id;
}

std::optional<QDate> Travel::getStart() {
    if (travelBookings.empty()) {
        return std::nullopt;
    }
    const auto min = std::ranges::min_element(
        travelBookings.begin(), travelBookings.end(),
        [](std::shared_ptr<Booking> a, std::shared_ptr<Booking> b) {
            return parseDate(a->getFromDate()) < parseDate(b->getFromDate());
        }
    );

    return parseDate((*min)->getFromDate());
}

std::string Travel::getStartString() {
    if (travelBookings.empty()) {
        return "";
    }
    const auto min = std::ranges::min_element(
        travelBookings.begin(), travelBookings.end(),
        [](std::shared_ptr<Booking> a, std::shared_ptr<Booking> b) {
            return parseDate(a->getFromDate()) < parseDate(b->getFromDate());
        }
    );

    return formatDate((*min)->getFromDate());
}

std::optional<QDate> Travel::getEnd() {
    if (travelBookings.empty()) {
        return std::nullopt;
    }
    const auto max = std::ranges::max_element(
        travelBookings.begin(), travelBookings.end(),
        [](std::shared_ptr<Booking> a, std::shared_ptr<Booking> b) {
            return parseDate(a->getToDate()) < parseDate(b->getToDate());
        }
    );

    return parseDate((*max)->getToDate());
}


std::string Travel::getEndString() {
    if (travelBookings.empty()) {
        return "";
    }
    const auto max = std::ranges::max_element(
        travelBookings.begin(), travelBookings.end(),
        [](std::shared_ptr<Booking> a, std::shared_ptr<Booking> b) {
            return parseDate(a->getFromDate()) < parseDate(b->getFromDate());
        }
    );

    return formatDate((*max)->getToDate());
}

std::shared_ptr<Graph<std::shared_ptr<Booking>, 100> > &Travel::getGraph() {
    if (!graph.has_value()) {
        buildGraph();
    }

    return *graph;
}

void Travel::serializeAll(nlohmann::json &json, serde::Encoder *encoder) {
    auto self = shared_from_this();
    serde_objects::Codec<std::shared_ptr<Travel> >::serialize(self, encoder);

    for (auto booking: travelBookings) {
        auto bookingEncoder = encoder->clone();
        serializeBooking(booking, bookingEncoder, json);
        delete bookingEncoder;
    }
}

void Travel::intoGeoJsonElements(std::vector<std::unique_ptr<GeoJsonElement> > &vector) {
    for (const auto booking: this->travelBookings) booking->intoGeoJsonElements(vector);
}

void Travel::buildGraph() {
    graph = std::make_shared<Graph<std::shared_ptr<Booking>, MAX_BOOKINGS> >();

    const auto editable = *graph;
    int i = 0;
    std::unordered_map<std::string, int> idMapping;
    for (const auto travel_booking: travelBookings) {
        editable->insertVertex(++i, travel_booking);
        idMapping[travel_booking->getId()] = i;
    }
    for (const auto travel_booking: travelBookings) {
        const int id = idMapping[travel_booking->getId()];
        for (const auto predecessor: travel_booking->getPredecessors()) {
            const int predecessorId = idMapping[predecessor];
            editable->insertEdge(predecessorId, id);
        }
    }
}

void serde_objects::Codec<std::shared_ptr<Travel> >::serialize(std::shared_ptr<Travel> &obj, serde::Encoder *encoder) {
    encoder->encode<const long>("travelId", obj->getId())
            .encode<const long>("customerId", obj->getCustomerId());
}

std::shared_ptr<Travel> serde_objects::Codec<std::shared_ptr<Travel> >::deserialize(serde::Decoder *decoder) {
    return std::make_shared<Travel>(
        decoder->at<const long>("travelId"),
        decoder->at<const long>("customerId")
    );
}
