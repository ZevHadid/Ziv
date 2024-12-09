#ifndef HELPER_HEADER_HPP
#define HELPER_HEADER_HPP

#include <algorithm>
#include <cstring>
#include <sstream>
#include <variant>

std::string sourcecode_filepath;

using EntityType = std::variant<int, bool, float, std::string>;

enum class instruction_type : uint8_t {
    VARIABLE,
    MODIFY, 
    ARGUMENT,
    FUNCTION,
    PRINT,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    CONCAT
};

enum class rvalue_type : uint8_t {
    INFERRED,
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN
};

#pragma pack(push, 1)
struct VariableStructure {
    instruction_type instructionType;
    std::string varName;
    rvalue_type dataType;
    std::variant<int, bool, float, std::string> value;
};

struct InstructionStructure {
    instruction_type instructionType;
    rvalue_type dataType;
    EntityType instructionArgument;
};
#pragma pack(pop)

bool isInteger(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool isFloat(const std::string& s) {
    std::istringstream iss(s);
    float f;
    return (iss >> f) && iss.eof();
}

void writeString(std::ofstream& outFile, const std::string& str) {
    // Calculate the length of the string
    size_t length = str.size();

    // Reserve space in a buffer for the length and the string
    std::vector<char> buffer(sizeof(length) + length);

    // Copy the length into the buffer
    std::memcpy(buffer.data(), &length, sizeof(length));

    // Copy the string data into the buffer
    std::memcpy(buffer.data() + sizeof(length), str.data(), length);

    // Write the entire buffer to the file in one go
    outFile.write(buffer.data(), buffer.size());
}

void readString(std::ifstream& inFile, std::string& str) {
    // Read the length of the string (assuming it is stored as a size_t)
    std::size_t length;
    inFile.read(reinterpret_cast<char*>(&length), sizeof(length));      

    // Resize the string to hold the data
    str.resize(length);

    // Read the string data
    inFile.read(&str[0], length);
}

void readString(std::ifstream& inFile, EntityType& value) {
    // Read the length of the string
    size_t length = 0;
    inFile.read(reinterpret_cast<char*>(&length), sizeof(length));

    // If the length is 0, assign an empty string to the variant
    if (length == 0) {
        value = std::string();  // Assign an empty string to the variant
        return;
    }

    // Create a buffer to hold the string data
    std::vector<char> buffer(length);
    
    // Read the string content into the buffer
    inFile.read(buffer.data(), length);
    
    // Create a string from the buffer and assign it to the variant
    value = std::string(buffer.data(), length);
}

#endif
