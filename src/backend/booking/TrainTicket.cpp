#include "TrainTicket.h"

#include "FlightBooking.h"
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

TrainTicket::TrainTicket(const std::string &id, const double price, const std::string &fromDate,
                         const std::string &toDate, std::string arrivalTime, std::string departureTime,
                         std::string fromStation,
                         std::string toStation,
                         const std::vector<std::string> &connectingStations,
                         TicketType type): Booking(id, price, fromDate, toDate),
                                           fromStation(std::move(fromStation)),
                                           toStation(std::move(toStation)),
                                           arrivalTime(std::move(arrivalTime)),
                                           departureTime(std::move(departureTime)),
                                           connectingStations(connectingStations),
                                           ticketType(type) {
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

std::vector<std::string> &TrainTicket::getConnectingStations() {
    return connectingStations;
}

void TrainTicket::setConnectingStations(std::vector<std::string> connectingStations) {
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
            out << *station;
            if (station != this->connectingStations.end() - 1) {
                out << ", ";
            }
        }
    }

    out << ". Preis: " << this->price << " Euro";

    return out.str();
}

void TrainTicket::showEditor(ChangeController *changeController) {
    (new TrainTicketUi(this, changeController))->show();
}

void serde_objects::Codec<TrainTicket *>::serialize(TrainTicket *&obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("id", obj->getId())
            .encode<const double>("price", obj->getPrice())
            .encode<const std::string>("fromDate", obj->getFromDate())
            .encode<const std::string>("toDate", obj->getToDate())
            .encode<const std::string>("arrivalTime", obj->getArrivalTime())
            .encode<const std::string>("departureTime", obj->getDepartureTime())
            .encode<const std::string>("fromStation", obj->getFromStation())
            .encode<const std::string>("toStation", obj->getToStation())
            .encode<const std::vector<std::string>>("connectingStations", obj->getConnectingStations())
            .encode<TicketType>("ticketType", obj->getTicketType());
}

TrainTicket *serde_objects::Codec<TrainTicket *>::deserialize(serde::Decoder *decoder) {
    return new TrainTicket(
        decoder->at<std::string>("id", {serde::validate::assertNotEmpty("ID cannot be empty")}),
        decoder->at<double>("price", {serde::validate::assertNotNan("Price cannot be NaN")}),
        decoder->at<std::string>("fromDate", {serde::validate::assertNotEmpty("From date cannot be empty")}),
        decoder->at<std::string>("toDate", {serde::validate::assertNotEmpty("To date cannot be empty")}),
        decoder->at<std::string>("arrivalTime", {serde::validate::assertNotEmpty("Arrival time cannot be empty")}),
        decoder->at<std::string>("departureTime", {serde::validate::assertNotEmpty("Departure time cannot be empty")}),
        decoder->at<std::string>("fromStation", {serde::validate::assertNotEmpty("From station cannot be empty")}),
        decoder->at<std::string>("toStation", {serde::validate::assertNotEmpty("To station cannot be empty")}),
        decoder->at<std::vector<std::string> >("connectingStations", {
                                                   serde::validate::indexed({
                                                       serde::validate::assertNotEmpty(
                                                           "Connecting station cannot be empty")
                                                   })
                                               }),
        decoder->at<TicketType>("ticketType")
    );
}
