#include "BinaryFormatReader.h"
#include <sstream>

BinaryFormatReader::BinaryFormatReader(const std::string &filename) : file(filename, std::ios::binary | std::ios::ate) {
    this->size = this->file.tellg();
    this->file.seekg(0, std::ios::beg);
}

char BinaryFormatReader::readChar() {
    char c;
    this->file.read(&c, 1);
    return c;
}

double BinaryFormatReader::readDouble() {
    double d;
    this->file.read(reinterpret_cast<char *>(&d), sizeof(d));
    return d;
}

int BinaryFormatReader::readInt() {
    int i;
    this->file.read(reinterpret_cast<char *>(&i), sizeof(i));
    return i;
}

std::string BinaryFormatReader::readString(const int length) {
    std::string s;
    s.resize(length);
    this->file.read(&s[0], length);
    return s;
}

std::string BinaryFormatReader::readPaddedString(const int length) {
    auto string = this->readString(length);

    while (string.back() == ' ') {
        string.pop_back();
    }

    return string;
}

bool BinaryFormatReader::isAtEnd() {
    return this->file.tellg() >= this->size;
}

std::string BinaryFormatReader::getDebugInfo() {
    std::stringstream out;

    out << "File position: " << this->file.tellg() << "/" << this->size << " bytes. " << std::endl;
    out << std::endl;
    out << "Currently at end: " << this->isAtEnd() << std::endl;

    return out.str();
}
