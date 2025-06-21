#pragma once

#include "GeoJsonElement.h"
#include "../backend/coord/Position.h"
#include <vector>
#include <utility>

class GeoJsonLine final : public GeoJsonElement {
    std::vector<std::pair<double, double> > points;

public:
    explicit GeoJsonLine(const std::vector<std::pair<double, double> > &path);

    void writeInto(nlohmann::json &el) override;

    static std::unique_ptr<GeoJsonLine> polyLine(std::initializer_list<std::pair<double, double> > pos) {
        return std::make_unique<GeoJsonLine>(pos);
    }

    static std::unique_ptr<GeoJsonLine> polyLine(std::vector<std::pair<double, double> > pos) {
        return std::make_unique<GeoJsonLine>(pos);
    }
};
