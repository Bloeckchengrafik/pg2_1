#pragma once
#include <vector>

#include "Codec.h"

namespace serde_objects {
    template<typename T>
    struct Codec<const std::vector<T> > {
        static void serialize(const std::vector<T> &obj, serde::Encoder *encoder);

        static const  std::vector<T> deserialize(serde::Decoder *decoder);
    };


    template<typename T>
    void Codec<const std::vector<T> >::serialize(const std::vector<T> &obj, serde::Encoder *encoder) {
        const auto vecEncoder = encoder->vec();
        for (auto item: obj) {
            Codec<T>::serialize(item, vecEncoder);
            vecEncoder->vec();
        }
    }

    template<typename T>
    const std::vector<T> Codec<const std::vector<T> >::deserialize(serde::Decoder *decoder) {
        auto vecDecoder = decoder->vec();
        std::vector<T> target{};
        while (!vecDecoder->isAtEnd()) {
            target.push_back(Codec<T>::deserialize(vecDecoder));
            vecDecoder->vec();
        }
        return target;
    }

    template<typename T>
    struct Codec<std::vector<T>> {
        static void serialize(std::vector<T>& obj, serde::Encoder* encoder);
        static std::vector<T> deserialize(serde::Decoder* decoder);
    };

    template<typename T>
    void Codec<std::vector<T>>::serialize(std::vector<T>& obj, serde::Encoder* encoder) {
        const auto vecEncoder = encoder->vec();
        for (auto& item : obj) {
            Codec<T>::serialize(item, vecEncoder);
            vecEncoder->vec();
        }
    }

    template<typename T>
    std::vector<T> Codec<std::vector<T>>::deserialize(serde::Decoder* decoder) {
        auto vecDecoder = decoder->vec();
        std::vector<T> target{};
        while (!vecDecoder->isAtEnd()) {
            target.push_back(Codec<T>::deserialize(vecDecoder));
            vecDecoder->vec();
        }
        return target;
    }

}
