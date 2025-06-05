#pragma once
#include <nlohmann/json.hpp>

#include "Codec.h"

namespace serde::json {
    class JsonEncoder final : public Encoder {
        nlohmann::json json;
        JsonEncoder *parent;
        std::vector<JsonEncoder *> children{};
        std::optional<std::string> parentKey;

    public:
        JsonEncoder(JsonEncoder *parent = nullptr, std::optional<std::string> parentKey = std::nullopt) : parent(parent),
            parentKey(std::move(parentKey)) {}

        ~JsonEncoder() override;

        void encodeInt(int value) override;

        void encodeDouble(double value) override;

        void encodeString(const std::string &value) override;

        Encoder *vec() override;

        Encoder *key(const std::string &key) override;

        nlohmann::json &getJson();

        void encodeLong(long value) override;

        Encoder * clone() override;

        void finish();
    };

    class JsonDecoder final : public Decoder {
        nlohmann::json json;
        std::vector<Decoder *> children{};

    public:
        explicit JsonDecoder(const nlohmann::json &json) : json(json) {
        }

        ~JsonDecoder() override;

        int decodeInt() override;

        double decodeDouble() override;

        std::string decodeString() override;

        Decoder *vec() override;

        Decoder *key(const std::string &key) override;

        bool isAtEnd() override;

        long decodeLong() override;
    };

    class IteratingJsonDecoder final : public Decoder {
        nlohmann::json::iterator start;
        nlohmann::json::iterator end;

    public:
        explicit IteratingJsonDecoder(const nlohmann::json::iterator &start, const nlohmann::json::iterator &end)
            : start(start), end(end) {
        }

        int decodeInt() override;

        double decodeDouble() override;

        std::string decodeString() override;

        Decoder *vec() override;

        Decoder *key(const std::string &key) override;

        bool isAtEnd() override;

        long decodeLong() override;
    };
}
