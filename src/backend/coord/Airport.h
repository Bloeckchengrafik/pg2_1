#pragma once
#include <string>
#include "../../serde/prelude.h"

class Airport {
    const std::string name;
    const std::string municipality;
    const std::string iso;
    const std::string code;

public:
    explicit Airport(const std::string &name, const std::string &municipality, const std::string &iso, const std::string &code);
    std::string getName() const;
    std::string getMunicipality() const;
    std::string getIso() const;
    std::string getCode() const;
};

template<> struct serde_objects::Codec<std::shared_ptr<Airport>> {
    static void serialize(const std::shared_ptr<Airport> &obj, serde::Encoder *encoder);
    static std::shared_ptr<Airport> deserialize(serde::Decoder *decoder);
};
