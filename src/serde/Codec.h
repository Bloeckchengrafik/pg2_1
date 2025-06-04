#pragma once
#include <functional>
#include <string>

namespace serde_objects {
    // Forward Declaration
    template<typename T>
    struct Codec;
}

namespace serde {
    template<typename T>
    using Validator = std::function<void(T&)>;

    class ValidationException final : public std::exception {
        std::string message;

    public:
        explicit ValidationException(const std::string &message) : message(message) {
        }

        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    class Encoder {
    public:
        virtual ~Encoder() = default;

        virtual void encodeInt(int value) = 0;

        virtual void encodeDouble(double value) = 0;

        virtual void encodeString(const std::string &value) = 0;

        virtual Encoder* vec() = 0;

        virtual Encoder* key(const std::string &key) = 0;

        template<typename T>
        Encoder& encode(const std::string &key, const T &value) {
            Encoder* encoder = this->key(key);
            serde_objects::Codec<T>::serialize(value, encoder);
            delete encoder;
            return *this;
        }
    };

    class Decoder {
    public:
        virtual ~Decoder() = default;

        virtual int decodeInt() = 0;

        virtual double decodeDouble() = 0;

        virtual std::string decodeString() = 0;

        virtual Decoder *vec() = 0;

        virtual Decoder *key(const std::string &key) = 0;

        virtual bool isAtEnd() = 0;

        template<typename T>
        T at(const std::string &key, std::initializer_list<Validator<T>> validators) {
            Decoder *decoder = this->key(key);
            T result = serde_objects::Codec<T>::deserialize(decoder);
            delete decoder;

            for (auto validator : validators) {
                validator(result);
            }

            return result;
        }

        template<typename T>
        T at(const std::string &key) {
            return at<T>(key, {});
        }
    };
}
