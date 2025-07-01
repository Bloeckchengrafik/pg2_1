//
// Created by chris on 6/21/25.
//

#include "GeoJsonLine.h"

GeoJsonLine::GeoJsonLine(const std::vector<std::pair<double, double>> &path) : points(path) {
}

void GeoJsonLine::writeInto(nlohmann::json &el) {
    el["type"] = "Feature";
    el["geometry"]["type"] = "LineString";
    el["geometry"]["coordinates"] = nlohmann::json::array();
    for (const auto &[lat, lon] : points) {
        el["geometry"]["coordinates"].push_back({lon, lat});
    }

    el["properties"]["_type"] = "Line";
}
