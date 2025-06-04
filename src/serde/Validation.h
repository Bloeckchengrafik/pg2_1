#pragma once

#include "Codec.h"

namespace serde::validate {
    Validator<const std::string> assertNotEmpty(const std::string &message);

    Validator<const std::string> assertLength(int length, const std::string &message);

    Validator<const double> assertNotNan(const std::string &message);

    template<typename T>
    Validator<const std::vector<T> > indexed(std::initializer_list<Validator<const T> > validators) {
        return [=](const std::vector<T> elements) {
            for (int i = 0; i < elements.size(); ++i) {
                try {
                    for (auto validator: validators) {
                        validator(elements[i]);
                    }
                } catch (const ValidationException &e) {
                    throw ValidationException(std::string(e.what()) + " at index " + std::to_string(i));
                }
            }
        };
    }
}
