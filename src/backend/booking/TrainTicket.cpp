#include "TrainTicket.h"
#include "../../ui/trainticketui.h"

TrainTicket::TrainTicket(const std::string &id, const double price, const std::string &fromDate,
                         const std::string &toDate, std::string arrivalTime, std::string departureTime,
                         std::string fromStation,
                         std::string toStation,
                         const std::vector<std::string> &connectingStations): Booking(id, price, fromDate, toDate),
                                                                              fromStation(std::move(fromStation)),
                                                                              toStation(std::move(toStation)),
                                                                              arrivalTime(std::move(arrivalTime)),
                                                                              departureTime(std::move(departureTime)),
                                                                              connectingStations(connectingStations) {
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

void TrainTicket::showEditor() {
    (new TrainTicketUi(this))->show();
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
            .encode<const std::vector<std::string>>("connectingStations", obj->getConnectingStations());
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
        decoder->at<std::vector<std::string> >("connectingStations", {serde::validate::indexed({serde::validate::assertNotEmpty("Connecting station cannot be empty")})})
    );
}
