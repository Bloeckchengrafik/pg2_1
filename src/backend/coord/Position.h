#pragma once

#include "../../serde/prelude.h"

template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) { // NOLINT(*-explicit-constructor)
        std::copy_n(str, N, value);
    }

    char value[N]{};

    std::string toString() const {
        return std::string(value);
    }
};

template <StringLiteral LA, StringLiteral LO>
struct Position {
    double latitude;
    double longitude;

    void serialize(serde::Encoder *encoder) {
        encoder->encode<double>(LA.toString(), latitude);
        encoder->encode<double>(LO.toString(), longitude);
    }

    static Position deserialize(serde::Decoder *decoder) {
        return Position{
            decoder->at<double>(LA.toString()),
            decoder->at<double>(LO.toString())
        };
    }

    std::pair<double, double> toPair() const {
        return { latitude, longitude };
    }
};
