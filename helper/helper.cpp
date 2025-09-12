#include "helper.hpp"
#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>

std::string sourcecode_filepath;

bool isInteger(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool isFloat(const std::string& s) {
    std::istringstream iss(s);
    float f;
    return (iss >> f) && iss.eof();
}

void writeString(std::ofstream& outFile, const std::string& str) {
    size_t length = str.size();
    std::vector<char> buffer(sizeof(length) + length);
    std::memcpy(buffer.data(), &length, sizeof(length));
    std::memcpy(buffer.data() + sizeof(length), str.data(), length);
    outFile.write(buffer.data(), buffer.size());
}

void readString(std::ifstream& inFile, std::string& str) {
    std::size_t length;
    inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
    str.resize(length);
    inFile.read(&str[0], length);
}

void readString(std::ifstream& inFile, EntityType& value) {
    size_t length = 0;
    inFile.read(reinterpret_cast<char*>(&length), sizeof(length));

    if (length == 0) {
        value = std::string();
        return;
    }

    std::vector<char> buffer(length);
    inFile.read(buffer.data(), length);
    value = std::string(buffer.data(), length);
}
