#include "Primitives.h"

void serde_objects::Codec<int>::serialize(int &obj, serde::Encoder *encoder) {
    encoder->encodeInt(obj);
}

int serde_objects::Codec<int>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeInt();
}

void serde_objects::Codec<double>::serialize(double &obj, serde::Encoder *encoder) {
    encoder->encodeDouble(obj);
}

double serde_objects::Codec<double>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeDouble();
}

void serde_objects::Codec<const int>::serialize(const int &obj, serde::Encoder *encoder) {
    encoder->encodeInt(obj);
}

const int serde_objects::Codec<const int>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeInt();
}

void serde_objects::Codec<const double>::serialize(const double &obj, serde::Encoder *encoder) {
    encoder->encodeDouble(obj);
}

const double serde_objects::Codec<const double>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeDouble();
}

void serde_objects::Codec<const long>::serialize(const double &obj, serde::Encoder *encoder) {
    encoder->encodeLong(obj);
}

const double serde_objects::Codec<const long>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeDouble();
}

void serde_objects::Codec<std::string>::serialize(std::string &obj, serde::Encoder *encoder) {
    encoder->encodeString(obj);
}

std::string serde_objects::Codec<std::string>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeString();
}

void serde_objects::Codec<const std::string>::serialize(const std::string &obj, serde::Encoder *encoder) {
    encoder->encodeString(obj);
}

const std::string serde_objects::Codec<const std::string>::deserialize(serde::Decoder *decoder) {
    return decoder->decodeString();
}
