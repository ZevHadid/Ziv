#pragma once

#include <string>
#include <variant>
#include <cstdint>
#include <fstream>

extern std::string sourcecode_filepath;

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

bool isInteger(const std::string& s);
bool isFloat(const std::string& s);
void writeString(std::ofstream& outFile, const std::string& str);
void readString(std::ifstream& inFile, std::string& str);
void readString(std::ifstream& inFile, EntityType& value);
