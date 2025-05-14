#pragma once
#include <fstream>

class BinaryFormatReader {
    std::ifstream file;
    long size;
public:
    explicit BinaryFormatReader(const std::string& filename);
    ~BinaryFormatReader() = default;

    char readChar();
    double readDouble();
    int readInt();
    std::string readString(int length);
    std::string readPaddedString(int length);

    bool isAtEnd();

    std::string getDebugInfo();
};
