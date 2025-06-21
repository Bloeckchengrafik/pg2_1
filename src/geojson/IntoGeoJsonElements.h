#pragma once
#include <memory>
#include <vector>

#include "GeoJsonElement.h"

class IntoGeoJsonElements {
public:
    virtual ~IntoGeoJsonElements() = default;
    virtual void intoGeoJsonElements(std::vector<std::unique_ptr<GeoJsonElement>> &vector) = 0;
};
