//
// Created by chris on 6/21/25.
//

#include "GeoJsonPin.h"

GeoJsonPin::GeoJsonPin(const std::string &name, const double lat, const double lon) : GeoJsonElement(), name(name), lat(lat), lon(lon) {
}

void GeoJsonPin::writeInto(nlohmann::json &el) {
    el["type"] = "Feature";
    el["properties"]["name"] = name;
    el["geometry"]["type"] = "Point";
    el["geometry"]["coordinates"] = nlohmann::json::array({lon, lat});
}
