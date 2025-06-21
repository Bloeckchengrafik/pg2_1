#pragma once
#include <vector>

#include "booking/Booking.h"
#include "../serde/prelude.h"


class Travel : public std::enable_shared_from_this<Travel>{
    long id;
    long customerId;
    std::vector<std::shared_ptr<Booking> > travelBookings{};

public:
    Travel(const long id, const long customerId) : id(id), customerId(customerId) {
    }

    long getId() const;

    long getCustomerId() const;

    void addBooking(std::shared_ptr<Booking> booking);

    std::vector<std::shared_ptr<Booking>> &getBookings();

    bool operator==(const Travel &other) const;

    std::string getStart();

    std::string getEnd();

    void serializeAll(nlohmann::json &json, serde::Encoder* encoder);
};

template<>
struct serde_objects::Codec<std::shared_ptr<Travel>> {
    static void serialize(std::shared_ptr<Travel> &obj, serde::Encoder *encoder);

    static std::shared_ptr<Travel> deserialize(serde::Decoder *decoder);
};
