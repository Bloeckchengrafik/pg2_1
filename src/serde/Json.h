#pragma once
#include <nlohmann/json.hpp>
#include <functional>

#include "Codec.h"

namespace serde::json {
    class AbstractJsonEncoder : public Encoder {
    public:
        virtual void finish() = 0;
    };

    typedef std::function<void(nlohmann::json value)> JsonSetterFunction;

    class JsonEncoder final : public AbstractJsonEncoder {
        nlohmann::json json;
        std::vector<AbstractJsonEncoder *> children{};
        std::optional<JsonSetterFunction> fn;

    public:
        JsonEncoder(std::optional<JsonSetterFunction> fn = std::nullopt) : fn(fn) {}

        ~JsonEncoder() override;

        void encodeInt(int value) override;

        void encodeDouble(double value) override;

        void encodeString(const std::string &value) override;

        Encoder *vec() override;

        Encoder *key(const std::string &key) override;

        nlohmann::json &getJson();

        void encodeLong(long value) override;

        Encoder * clone() override;

        void finish() override;
    };

    class IteratingJsonEncoder final : public AbstractJsonEncoder {
        nlohmann::json json = nlohmann::json::array();
        std::vector<AbstractJsonEncoder *> children{};
        std::optional<JsonSetterFunction> fn;

    public:
        IteratingJsonEncoder(std::optional<JsonSetterFunction> fn = std::nullopt) : fn(fn) {}

        ~IteratingJsonEncoder() override;

        void encodeInt(int value) override;

        void encodeDouble(double value) override;

        void encodeString(const std::string &value) override;

        Encoder *vec() override;

        Encoder *key(const std::string &key) override;

        nlohmann::json &getJson();

        void encodeLong(long value) override;

        Encoder * clone() override;

        void finish() override;
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