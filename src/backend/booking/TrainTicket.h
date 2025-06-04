#pragma once
#include "Booking.h"
#include "../../serde/prelude.h"


class TrainTicket final : public Booking {
    friend class TrainTicketUi;

    std::string fromStation;
    std::string toStation;
    std::string arrivalTime;
    std::string departureTime;
    std::vector<std::string> connectingStations;

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
        const std::vector<std::string> &connectingStations
    );

    std::string showDetails() override;

    void showEditor() override;

    std::string &getFromStation();

    std::string &getToStation();

    std::string &getArrivalTime();

    std::string &getDepartureTime();

    std::vector<std::string> &getConnectingStations();
};


template<> struct serde_objects::Codec<TrainTicket*> {
    static void serialize(TrainTicket* &obj, serde::Encoder *encoder);
    static TrainTicket* deserialize(serde::Decoder *decoder);
};
