#include "Airport.h"

Airport::Airport(const std::string &name, const std::string &municipality, const std::string &iso,
                 const std::string &code)
    : name(name),
      municipality(municipality),
      iso(iso),
      code(code) {
}

std::string Airport::getName() const {
    return name;
}

std::string Airport::getMunicipality() const {
    return municipality;
}

std::string Airport::getIso() const {
    return iso;
}

std::string Airport::getCode() const {
    return code;
}

void serde_objects::Codec<std::shared_ptr<Airport>>::serialize(const std::shared_ptr<Airport> &obj, serde::Encoder *encoder) {
    encoder->encode<const std::string>("name", obj->getName())
            .encode<const std::string>("iso_country", obj->getIso())
            .encode<const std::string>("iata_code", obj->getCode())
            .encode<const std::string>("municipality", obj->getMunicipality());
}

std::shared_ptr<Airport> serde_objects::Codec<std::shared_ptr<Airport>>::deserialize(serde::Decoder *decoder) {
    return std::make_shared<Airport>(
        decoder->at<std::string>("name"),
        decoder->at<std::string>("municipality"),
        decoder->at<std::string>("iso_country"),
        decoder->at<std::string>("iata_code")
    );
}
