#pragma once
#include <memory>

#include "coord/Airport.h"

class BookingController {
public:
    virtual ~BookingController() = default;

    virtual void onChange() = 0;
    virtual std::optional<std::shared_ptr<Airport>> getAirport(std::string& code) = 0;
};
