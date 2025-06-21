#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"
#include "../coord/Position.h"

enum TicketType {
    SUPER_SAVING_FIRST_CLASS,
    SUPER_SAVING_SECOND_CLASS,
    SAVING_FIRST_CLASS,
    SAVING_SECOND_CLASS,
    FLEX_FIRST_CLASS,
    FLEX_SECOND_CLASS,
};

template<> struct serde_objects::Codec<TicketType> {
    static void serialize(TicketType &obj, serde::Encoder *encoder);
    static TicketType deserialize(serde::Decoder *decoder);
};

typedef Position<"latitude", "longitude"> TrainTicketPosition;
typedef Position<"fromStationLatitude", "fromStationLongitude"> TrainStationFromPosition;
typedef Position<"toStationLatitude", "toStationLongitude"> TrainStationToPosition;

struct TicketConnectingStation {
    std::string station;
    TrainTicketPosition position;
};

template<> struct serde_objects::Codec<TicketConnectingStation> {
    static void serialize(TicketConnectingStation &obj, serde::Encoder *encoder);
    static TicketConnectingStation deserialize(serde::Decoder *decoder);
};

class TrainTicket final : public Booking {
    friend class TrainTicketUi;

    std::string fromStation;
    TrainStationFromPosition fromStationPosition;
    std::string toStation;
    TrainStationToPosition toStationPosition;
    std::string arrivalTime;
    std::string departureTime;
    std::vector<TicketConnectingStation> connectingStations;
    TicketType ticketType;

public:
    TrainTicket(
        const std::string &id,
        const double price,
        const std::string &fromDate,
        const std::string &toDate,
        std::string arrivalTime,
        std::string departureTime,
        std::string fromStation,
        std::string toStation,
        const std::vector<TicketConnectingStation> &connectingStations,
        TicketType ticketType,
        TrainStationFromPosition fromStationPosition,
        TrainStationToPosition toStationPosition
    );

    std::string showDetails() override;

    void showEditor(ChangeController *changeController) override;

    std::string &getFromStation();

    std::string &getToStation();

    std::string &getArrivalTime();

    std::string &getDepartureTime();

    std::vector<TicketConnectingStation> &getConnectingStations();

    TrainStationFromPosition &getFromStationPosition();
    TrainStationToPosition &getToStationPosition();

    void setConnectingStations(std::vector<TicketConnectingStation> connectingStations);
    void setFromStation(std::string fromStation);
    void setToStation(std::string toStation);
    void setArrivalTime(std::string arrivalTime);
    void setDepartureTime(std::string departureTime);
    void setTicketType(TicketType ticketType);

    QIcon getIcon() override;

    TicketType & getTicketType();
};


template<> struct serde_objects::Codec<TrainTicket*> {
    static void serialize(TrainTicket* &obj, serde::Encoder *encoder);
    static TrainTicket* deserialize(serde::Decoder *decoder);
};
