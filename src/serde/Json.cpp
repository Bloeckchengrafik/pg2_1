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
    JsonSetterFunction setter = [&](nlohmann::json j) {
        json = j;
    };
    auto *encoder = new IteratingJsonEncoder(std::optional<JsonSetterFunction>(setter));
    children.push_back(encoder);
    return encoder;
}

serde::Encoder *serde::json::JsonEncoder::key(const std::string &key) {
    if (!json.is_object()) {
        json = nlohmann::json::object();
    }
    
    // Convert the lambda to a function pointer
    JsonSetterFunction setter = [this, cap_key = key](nlohmann::json j) {
        this->json[cap_key] = j;
    };
    auto *encoder = new JsonEncoder(std::optional<JsonSetterFunction>(setter));
    children.push_back(encoder);
    json[key] = encoder->json;
    return encoder;
}

nlohmann::json &serde::json::JsonEncoder::getJson() {
    for (const auto child : children) child->finish();
    return json;
}

void serde::json::JsonEncoder::encodeLong(long value) {
    json = value;
}

serde::Encoder * serde::json::JsonEncoder::clone() {
    for (const auto child : children) child->finish();
    auto *encoder = new JsonEncoder();
    encoder->json = json;
    return encoder;
}

void serde::json::JsonEncoder::finish() {
    for (const auto child : children) child->finish();
    children.clear();

    if (fn == std::nullopt) {
        return;
    }

    (*fn)(json);
}

//iter

serde::json::IteratingJsonEncoder::~IteratingJsonEncoder() {
    for (const auto child : children) delete child;
}

void serde::json::IteratingJsonEncoder::encodeInt(int value) {
    json.push_back(value);
}

void serde::json::IteratingJsonEncoder::encodeDouble(double value) {
    json.push_back(value);
}

void serde::json::IteratingJsonEncoder::encodeString(const std::string &value) {
    json.push_back(value);
}

serde::Encoder *serde::json::IteratingJsonEncoder::vec() {
    return this;
}

serde::Encoder *serde::json::IteratingJsonEncoder::key(const std::string &key) {
    throw std::runtime_error("not implemented");
}

nlohmann::json &serde::json::IteratingJsonEncoder::getJson() {
    for (const auto child : children) child->finish();
    return json;
}

void serde::json::IteratingJsonEncoder::encodeLong(long value) {
    json.push_back(value);
}

serde::Encoder * serde::json::IteratingJsonEncoder::clone() {
    return this;
}

void serde::json::IteratingJsonEncoder::finish() {
    for (const auto child : children) child->finish();
    children.clear();

    if (fn == std::nullopt) {
        return;
    }

    (*fn)(json);
}
//end

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

long serde::json::JsonDecoder::decodeLong() {
    return json.get<long>();
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
    ++start;
    if (start == end) return nullptr;
    current = *start;
    return nullptr;
}

serde::Decoder * serde::json::IteratingJsonDecoder::key(const std::string &key) {
    if (!current.has_value()) {
        if (start == end) return nullptr;
        current = *start;
    }

    return new JsonDecoder(current.value().at(key));
}

bool serde::json::IteratingJsonDecoder::isAtEnd() {
    return start == end;
}

long serde::json::IteratingJsonDecoder::decodeLong() {
    return (start++)->get<long>();
}