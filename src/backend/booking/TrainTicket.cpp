#include "TrainTicket.h"

#include "FlightBooking.h"
#include "../../geojson/GeoJsonLine.h"
#include "../../geojson/GeoJsonPin.h"
#include "../../ui/trainticketui.h"

void serde_objects::Codec<TicketType>::serialize(TicketType &obj, serde::Encoder *encoder) {
    switch (obj) {
        case SUPER_SAVING_FIRST_CLASS:
            encoder->encodeString("SSP1");
            break;
        case SUPER_SAVING_SECOND_CLASS:
            encoder->encodeString("SSP2");
            break;
        case SAVING_FIRST_CLASS:
            encoder->encodeString("SP1");
            break;
        case SAVING_SECOND_CLASS:
            encoder->encodeString("SP2");
            break;
        case FLEX_FIRST_CLASS:
            encoder->encodeString("FP1");
            break;
        case FLEX_SECOND_CLASS:
            encoder->encodeString("FP2");
            break;
    }
}

TicketType serde_objects::Codec<TicketType>::deserialize(serde::Decoder *decoder) {
    const auto str = decoder->decodeString();
    if (str == "SSP1") return SUPER_SAVING_FIRST_CLASS;
    if (str == "SSP2") return SUPER_SAVING_SECOND_CLASS;
    if (str == "SP1") return SAVING_FIRST_CLASS;
    if (str == "SP2") return SAVING_SECOND_CLASS;
    if (str == "FP1") return FLEX_FIRST_CLASS;
    if (str == "FP2") return FLEX_SECOND_CLASS;
    throw serde::ValidationException("Invalid ticket type code");
}

void serde_objects::Codec<TicketConnectingStation>::serialize(TicketConnectingStation &obj, serde::Encoder *encoder) {
    encoder->key("stationName")->encodeString(obj.station);
    obj.position.serialize(encoder);
}

TicketConnectingStation serde_objects::Codec<TicketConnectingStation>::deserialize(serde::Decoder *decoder) {
    return {
        .station = decoder->at<std::string>("stationName", {
                                                serde::validate::assertNotEmpty("Station name cannot be empty")
                                            }),
        .position = TrainTicketPosition::deserialize(decoder)
    };
}

TrainTicket::TrainTicket(const std::string &id, const double price, const std::string &fromDate,
                         const std::string &toDate, std::vector<std::string> predecessors,
                         std::string arrivalTime, std::string departureTime,
                         std::string fromStation, std::string toStation,
                         const std::vector<TicketConnectingStation> &connectingStations,
                         TicketType type,
                         TrainStationFromPosition fromStationPosition,
                         TrainStationToPosition toStationPosition
): Booking(id, price, fromDate, toDate, predecessors),
   fromStation(std::move(fromStation)),
   toStation(std::move(toStation)),
   arrivalTime(std::move(arrivalTime)),
   departureTime(std::move(departureTime)),
   connectingStations(connectingStations),
   ticketType(type),
   fromStationPosition(fromStationPosition),
   toStationPosition(toStationPosition) {
}

std::string &TrainTicket::getFromStation() {
    return fromStation;
}

std::string &TrainTicket::getToStation() {
    return toStation;
}

std::string &TrainTicket::getArrivalTime() {
    return arrivalTime;
}

std::string &TrainTicket::getDepartureTime() {
    return departureTime;
}

std::vector<TicketConnectingStation> &TrainTicket::getConnectingStations() {
    return connectingStations;
}

TrainStationFromPosition &TrainTicket::getFromStationPosition() {
    return fromStationPosition;
}

TrainStationToPosition &TrainTicket::getToStationPosition() {
    return toStationPosition;
}

void TrainTicket::setConnectingStations(std::vector<TicketConnectingStation> connectingStations) {
    this->connectingStations = std::move(connectingStations);
}

void TrainTicket::setFromStation(std::string fromStation) {
    this->fromStation = std::move(fromStation);
}

void TrainTicket::setToStation(std::string toStation) {
    this->toStation = std::move(toStation);
}

void TrainTicket::setArrivalTime(std::string arrivalTime) {
    this->arrivalTime = std::move(arrivalTime);
}

void TrainTicket::setDepartureTime(std::string departureTime) {
    this->departureTime = std::move(departureTime);
}

void TrainTicket::setTicketType(TicketType ticketType) {
    this->ticketType = ticketType;
}

QIcon TrainTicket::getIcon() {
    return QIcon(":/icons/train.svg");
}

TicketType &TrainTicket::getTicketType() {
    return ticketType;
}

void TrainTicket::intoGeoJsonElements(std::vector<std::unique_ptr<GeoJsonElement> > &vector) {
    std::vector<std::pair<double, double> > pos;
    pos.push_back(this->fromStationPosition.toPair());
    vector.push_back(GeoJsonPin::pin(this->fromStation, this->fromStationPosition));
    for (auto [station, position]: connectingStations) {
        pos.push_back(position.toPair());
        vector.push_back(GeoJsonPin::pin(station, position));
    }
    pos.push_back(this->toStationPosition.toPair());
    vector.push_back(GeoJsonPin::pin(this->toStation, this->toStationPosition));
    vector.push_back(GeoJsonLine::polyLine(pos));
}

std::string TrainTicket::showDetails() {
    std::stringstream out;
    out << "Zugticket von "
            << this->fromStation
            << " nach "
            << this->toStation
            << " am " << formatDate(this->fromDate)
            << " von " << this->departureTime
            << " bis " << this->arrivalTime;

    if (!this->connectingStations.empty()) {
        out << " über ";

        for (auto station = this->connectingStations.begin(); station != this->connectingStations.end(); ++station) {
            out << station->station;
            if (station != this->connectingStations.end() - 1) {
                out << ", ";
            }
        }
    }

    out << ". Preis: " << this->price << " Euro";

    return out.str();
}

void TrainTicket::showEditor(const std::shared_ptr<BookingController> changeController) {
    (new TrainTicketUi(std::static_pointer_cast<TrainTicket>(shared_from_this()), changeController))->show();
}

void serde_objects::Codec<std::shared_ptr<TrainTicket> >::serialize(std::shared_ptr<TrainTicket> &obj,
                                                                    serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("arrivalTime", obj->getArrivalTime())
            .encode<const std::string>("departureTime", obj->getDepartureTime())
            .encode<const std::string>("fromStation", obj->getFromStation())
            .encode<const std::string>("toStation", obj->getToStation())
            .encode<const std::vector<TicketConnectingStation>>("connectingStations", obj->getConnectingStations())
            .encode<TicketType>("ticketType", obj->getTicketType());

    obj->getFromStationPosition().serialize(encoder);
    obj->getToStationPosition().serialize(encoder);
    serializePredecessors(encoder, obj->getPredecessors());
}

std::shared_ptr<TrainTicket> serde_objects::Codec<std::shared_ptr<TrainTicket> >::deserialize(serde::Decoder *decoder) {
    return std::make_shared<TrainTicket>(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        deserializePredecessors(decoder),
        decoder->at<std::string>("arrivalTime", {serde::validate::assertNotEmpty("Arrival time cannot be empty")}),
        decoder->at<std::string>("departureTime", {serde::validate::assertNotEmpty("Departure time cannot be empty")}),
        decoder->at<std::string>("fromStation", {serde::validate::assertNotEmpty("From station cannot be empty")}),
        decoder->at<std::string>("toStation", {serde::validate::assertNotEmpty("To station cannot be empty")}),
        decoder->at<std::vector<TicketConnectingStation> >("connectingStations"),
        decoder->at<TicketType>("ticketType"),
        TrainStationFromPosition::deserialize(decoder),
        TrainStationToPosition::deserialize(decoder)
    );
}
