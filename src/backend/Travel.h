#pragma once
#include <vector>

#include "booking/Booking.h"
#include "../serde/prelude.h"


class Travel {
    long id;
    long customerId;
    std::vector<Booking *> travelBookings{};

public:
    Travel(const long id, const long customerId) : id(id), customerId(customerId) {
    }

    long getId() const;

    long getCustomerId() const;

    void addBooking(Booking *booking);

    std::vector<Booking *> &getBookings();

    bool operator==(const Travel &other) const;

    std::string getStart();

    std::string getEnd();
};

template<>
struct serde_objects::Codec<Travel *> {
    static void serialize(Travel * &obj, serde::Encoder *encoder);

    static Travel *deserialize(serde::Decoder *decoder);
};
