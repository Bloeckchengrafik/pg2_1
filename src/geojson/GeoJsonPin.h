#pragma once
#include "GeoJsonElement.h"
#include "../backend/coord/Position.h"

class GeoJsonPin final : public GeoJsonElement {
    std::string name;
    double lat;
    double lon;

public:
    GeoJsonPin(const std::string &name, double lat, double lon);

    void writeInto(nlohmann::json &) override;

    template<StringLiteral LA, StringLiteral LO>
    static std::unique_ptr<GeoJsonPin> pin(const std::string &name, Position<LA, LO> pos) {
        return std::make_unique<GeoJsonPin>(name, pos.latitude, pos.longitude);
    }
};
