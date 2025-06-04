#include "Json.h"

serde::json::JsonEncoder::~JsonEncoder() {
    for (const auto child : children) delete child;
}

void serde::json::JsonEncoder::encodeInt(int value) {
    json = value;
}

void serde::json::JsonEncoder::encodeDouble(double value) {
    json = value;
}

void serde::json::JsonEncoder::encodeString(const std::string &value) {
    json = value;
}

serde::Encoder *serde::json::JsonEncoder::vec() {
    json = nlohmann::json::array();
    auto *encoder = new JsonEncoder();
    children.push_back(encoder);
    return encoder;
}

serde::Encoder *serde::json::JsonEncoder::key(const std::string &key) {
    if (!json.is_object()) {
        json = nlohmann::json::object();
    }
    
    auto *encoder = new JsonEncoder();
    children.push_back(encoder);
    json[key] = encoder->json;
    return encoder;
}

nlohmann::json &serde::json::JsonEncoder::getJson() {
    return json;
}

serde::json::JsonDecoder::~JsonDecoder() {
    for (const auto child : children) delete child;
}

int serde::json::JsonDecoder::decodeInt() {
    return json.get<int>();
}

double serde::json::JsonDecoder::decodeDouble() {
    return json.get<double>();
}

std::string serde::json::JsonDecoder::decodeString() {
    return json.get<std::string>();
}

serde::Decoder *serde::json::JsonDecoder::vec() {
    auto *decoder = new IteratingJsonDecoder(json.begin(), json.end());
    children.push_back(decoder);
    return decoder;
}

serde::Decoder * serde::json::JsonDecoder::key(const std::string &key) {
    const auto child = json.at<nlohmann::json>(key);
    Decoder *decoder = new JsonDecoder(child);
    children.push_back(decoder);
    return decoder;
}

bool serde::json::JsonDecoder::isAtEnd() {
    return false;
}

int serde::json::IteratingJsonDecoder::decodeInt() {
    return (start++)->get<int>();
}

double serde::json::IteratingJsonDecoder::decodeDouble() {
    return (start++)->get<double>();
}

std::string serde::json::IteratingJsonDecoder::decodeString() {
    return (start++)->get<std::string>();
}

serde::Decoder * serde::json::IteratingJsonDecoder::vec() {
    throw std::runtime_error("Invalid operation");
}

serde::Decoder * serde::json::IteratingJsonDecoder::key(const std::string &key) {
    throw std::runtime_error("Invalid operation");
}

bool serde::json::IteratingJsonDecoder::isAtEnd() {
    return start == end;
}
