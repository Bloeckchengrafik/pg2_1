#pragma once
#include <vector>

#include "booking/Booking.h"


class Travel {
    long id;
    long customerId;
    std::vector<Booking*> travelBookings{};
public:
    Travel(const long id, const long customerId) : id(id), customerId(customerId) {}

    long getId() const;
};
