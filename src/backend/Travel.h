#pragma once
#include <vector>

#include "booking/Booking.h"
#include "../serde/prelude.h"
#include "digraph/Graph.h"

#define MAX_BOOKINGS 100

class Travel final : public std::enable_shared_from_this<Travel>, public IntoGeoJsonElements{
    long id;
    long customerId;
    std::vector<std::shared_ptr<Booking> > travelBookings{};
    std::optional<std::shared_ptr<Graph<std::shared_ptr<Booking>, MAX_BOOKINGS>>> graph{};

public:
    Travel(const long id, const long customerId) : id(id), customerId(customerId) {
    }

    long getId() const;

    long getCustomerId() const;

    void addBooking(std::shared_ptr<Booking> booking);

    std::vector<std::shared_ptr<Booking>> &getBookings();

    bool operator==(const Travel &other) const;

    std::optional<QDate> getStart();
    std::string getStartString();

    std::optional<QDate> getEnd();
    std::string getEndString();

    std::shared_ptr<Graph<std::shared_ptr<Booking>, MAX_BOOKINGS>> &getGraph();

    void serializeAll(nlohmann::json &json, serde::Encoder* encoder);

    void intoGeoJsonElements(std::vector<std::unique_ptr<GeoJsonElement>> &vector) override;

private:
    void buildGraph();
};

template<>
struct serde_objects::Codec<std::shared_ptr<Travel>> {
    static void serialize(std::shared_ptr<Travel> &obj, serde::Encoder *encoder);

    static std::shared_ptr<Travel> deserialize(serde::Decoder *decoder);
};
