#include "handle_variable_declaration.hpp"
#include "globals.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <variant>
#include "../helper/helper.hpp"

void handleVariableDeclaration(std::vector<std::string>& tokens) {

    VariableStructure variable;
    variable.instructionType = instruction_type::VARIABLE;
    variable.varName = tokens[1];

    std::ofstream outFile(sourcecode_filepath.substr(0, sourcecode_filepath.size() - 2) + "ef", std::ios::binary | std::ios::out | std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Write the instruction type and lvalue
    outFile.write(reinterpret_cast<const char*>(&variable.instructionType), sizeof(variable.instructionType));
    writeString(outFile, variable.varName);

    std::string rvalue = tokens[3];

    if (rvalue.front() == '"' && rvalue.back() == '"') {
        variable.dataType = rvalue_type::STRING;
        variable.value = rvalue;
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        writeString(outFile, std::get<std::string>(variable.value));
    }

    else if (isInteger(rvalue)) {
        variable.dataType = rvalue_type::INTEGER;
        variable.value = std::stoi(rvalue);
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        outFile.write(reinterpret_cast<const char*>(&std::get<int32_t>(variable.value)), sizeof(int32_t));
    }

    else if (isFloat(rvalue)) {
        variable.dataType = rvalue_type::FLOAT;
        variable.value = std::stof(rvalue);
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        outFile.write(reinterpret_cast<const char*>(&std::get<float>(variable.value)), sizeof(float));
    }

    else if (rvalue == "true") {
        variable.dataType = rvalue_type::BOOLEAN;
        variable.value = true;
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        outFile.write(reinterpret_cast<const char*>(&std::get<bool>(variable.value)), sizeof(bool));
    }

    else if (rvalue == "false") {
        variable.dataType = rvalue_type::BOOLEAN;
        variable.value = false;
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        outFile.write(reinterpret_cast<const char*>(&std::get<bool>(variable.value)), sizeof(bool));
    }

    else {
        variable.dataType = rvalue_type::INFERRED;
        variable.value = rvalue;
        outFile.write(reinterpret_cast<const char*>(&variable.dataType), sizeof(variable.dataType));
        writeString(outFile, std::get<std::string>(variable.value));
    }

    for (int i = 4; i < tokens.size(); ++i) {
        if (variable.dataType == rvalue_type::STRING && tokens[i] == "+") {
            instruction.instructionType = instruction_type::CONCAT;
            if (tokens[i + 1].front() == '"' && tokens[i + 1].back() == '"') instruction.dataType = rvalue_type::STRING;
            else instruction.dataType = rvalue_type::INFERRED;
            instruction.instructionArgument = tokens[i + 1];

            outFile.write(reinterpret_cast<const char*>(&instruction.instructionType), sizeof(instruction.instructionType));
            outFile.write(reinterpret_cast<const char*>(&instruction.dataType), sizeof(instruction.dataType));
            writeString(outFile, std::get<std::string>(instruction.instructionArgument));
        }
    }

    outFile.close();
}
