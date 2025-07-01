#include "CheckConfiguration.h"
#include <nlohmann/json.hpp>
#include <fstream>

CheckConfigurationController::CheckConfigurationController() {
    if (std::ifstream configFile("config.json"); configFile.is_open()) {
        nlohmann::json j;
        configFile >> j;
        serde::json::JsonDecoder decoder(j);
        config = serde_objects::Codec<std::shared_ptr<CheckConfiguration> >::deserialize(&decoder);
    } else {
        config = std::make_shared<CheckConfiguration>(true, true, true, true);
        saveConfig();
    }
}

std::shared_ptr<CheckConfiguration> CheckConfigurationController::getConfig() {
    return config;
}

void CheckConfigurationController::saveConfig() {
    std::ofstream configFile("config.json");
    serde::json::JsonEncoder encoder;
    serde_objects::Codec<std::shared_ptr<CheckConfiguration>>::serialize(config, &encoder);
    configFile << encoder.getJson();
    configFile.close();
}
