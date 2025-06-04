#include "Validation.h"

#include <cmath>
#include <stdexcept>

serde::Validator<const std::string> serde::validate::assertNotEmpty(const std::string &message) {
    return [=](const std::string &element) {
        if (element.empty()) {
            throw ValidationException(message);
        }
    };
}

serde::Validator<const std::string> serde::validate::assertLength(const int length, const std::string &message) {
    return [=](const std::string &element) {
        if (element.length() != length) {
            throw ValidationException(message);
        }
    };
}

serde::Validator<const double> serde::validate::assertNotNan(const std::string &message) {
    return [=](const double element) {
        if (std::isnan(element)) {
            throw ValidationException(message);
        }
    };
}
