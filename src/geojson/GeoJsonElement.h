#pragma once
#include <nlohmann/json.hpp>

class GeoJsonElement {
public:
    virtual ~GeoJsonElement() = default;

    virtual void writeInto(nlohmann::json&) = 0;
};
