#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"

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

class TrainTicket final : public Booking {
    friend class TrainTicketUi;

    std::string fromStation;
    std::string toStation;
    std::string arrivalTime;
    std::string departureTime;
    std::vector<std::string> connectingStations;
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
        const std::vector<std::string> &connectingStations,
        TicketType ticketType
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getFromStation();

    std::string &getToStation();

    std::string &getArrivalTime();

    std::string &getDepartureTime();

    std::vector<std::string> &getConnectingStations();

    QIcon getIcon() override;

    TicketType & getTicketType();
};


template<> struct serde_objects::Codec<TrainTicket*> {
    static void serialize(TrainTicket* &obj, serde::Encoder *encoder);
    static TrainTicket* deserialize(serde::Decoder *decoder);
};
