#pragma once

#include "../serde/prelude.h"

struct CheckConfiguration {
    bool travelOverlaps;
    bool tooManyRentalCars;
    bool tooManyHotels;
    bool tooLittleHotels;
};

template<>
struct serde_objects::Codec<std::shared_ptr<CheckConfiguration>> {
    static void serialize(std::shared_ptr<CheckConfiguration> &obj, serde::Encoder *encoder) {
        const int &travelOverlaps = obj->travelOverlaps;
        const int &tooManyRentalCars = obj->tooManyRentalCars;
        const int &tooManyHotels = obj->tooManyHotels;
        const int &tooLittleHotels = obj->tooLittleHotels;
        encoder->encode("travelOverlaps", travelOverlaps);
        encoder->encode("tooManyRentalCars", tooManyRentalCars);
        encoder->encode("tooManyHotels", tooManyHotels);
        encoder->encode("tooLittleHotels", tooLittleHotels);
    }

    static std::shared_ptr<CheckConfiguration> deserialize(serde::Decoder *decoder) {
        return std::make_shared<CheckConfiguration> (
            decoder->at<int>("travelOverlaps") > 0,
            decoder->at<int>("tooManyRentalCars") > 0,
            decoder->at<int>("tooManyHotels") > 0,
            decoder->at<int>("tooLittleHotels") > 0
        );
    }
};

class CheckConfigurationController {
    std::shared_ptr<CheckConfiguration> config;

public:
    CheckConfigurationController();

    std::shared_ptr<CheckConfiguration> getConfig();
    void saveConfig();
};