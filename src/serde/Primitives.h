#pragma once

#include "Codec.h"

template<>
struct serde_objects::Codec<int> {
    static void serialize(int &obj, serde::Encoder *encoder);

    static int deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<double> {
    static void serialize(double &obj, serde::Encoder *encoder);

    static double deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<const int> {
    static void serialize(const int &obj, serde::Encoder *encoder);

    static const int deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<const double> {
    static void serialize(const double &obj, serde::Encoder *encoder);

    static const double deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<const long> {
    static void serialize(const double &obj, serde::Encoder *encoder);

    static const double deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<std::string> {
    static void serialize(std::string &obj, serde::Encoder *encoder);

    static std::string deserialize(serde::Decoder *decoder);
};

template<>
struct serde_objects::Codec<const std::string> {
    static void serialize(const std::string &obj, serde::Encoder *encoder);

    static const std::string deserialize(serde::Decoder *decoder);
};
