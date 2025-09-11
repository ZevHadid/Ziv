#ifndef BYTECODE_TRANSLATOR_HPP
#define BYTECODE_TRANSLATOR_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <numeric>
#include <cstdint>
#include "helper_header.hpp"    

size_t line;
InstructionStructure instruction;

void handlePrintStatement(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Error: invalid syntax." << std::endl;
        return;
    }

    instruction.instructionType = instruction_type::PRINT;
    instruction.instructionArgument = tokens[1];

    std::ofstream outFile(sourcecode_filepath.substr(0, sourcecode_filepath.size() - 2) + "ef", std::ios::binary | std::ios::out | std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&instruction.instructionType), sizeof(instruction.instructionType));
    writeString(outFile, std::get<std::string>(instruction.instructionArgument));

    outFile.close();
}

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

void handleFunctionDeclaration(std::vector<std::string>& tokens) {
    size_t leftParenthesisPos = 0;
    size_t rightParenthesisPos = 0;
    size_t leftCurlyBracePos = 0;
    size_t rightCurlyBracePos = 0;

    // Find the position of the opening parenthesis
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "(") {
            leftParenthesisPos = i;
            break;
        }
    }

    // Find the position of the closing parenthesis
    int parenthesisCount = 1;
    for (size_t i = leftParenthesisPos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "(") {
            parenthesisCount++;
        } else if (tokens[i] == ")") {
            parenthesisCount--;
            if (parenthesisCount == 0) {
                rightParenthesisPos = i;
                break;
            }
        }
    }

    // Find the position of the opening curly brace
    for (size_t i = rightParenthesisPos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "{") {
            leftCurlyBracePos = i;
            break;
        }
    }

    // Find the position of the closing curly brace
    int curlyBraceCount = 1;
    for (size_t i = leftCurlyBracePos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "{") {
            curlyBraceCount++;
        } else if (tokens[i] == "}") {
            curlyBraceCount--;
            if (curlyBraceCount == 0) {
                rightCurlyBracePos = i;
                break;
            }
        }
    }

    // TODO: use position variables to extract the function declaration from the possible variable declaration
    
}

void translateLine(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return;
    }
    
    if (tokens[0] == "func") {
        handleFunctionDeclaration(tokens);
    } else if (tokens.size() > 2 && tokens[0] == "var" && tokens[2] == "=") {
        handleVariableDeclaration(tokens);
    } else if (tokens[0] == "print") {
        handlePrintStatement(tokens);
    } else {
        std::cout << "Invalid syntax. What even is that?";
        std::exit(0);
    }
}

std::vector<std::string> tokenizeStatement(const std::string& statement) {
    std::vector<std::string> tokens;
    std::string token;
    bool insideQuotes = false;

    for (size_t i = 0; i < statement.length(); ++i) {
        char c = statement[i];

        if (c == '"') {
            insideQuotes = !insideQuotes;
            token += c;
        } else if (insideQuotes) {
            token += c;
        } else if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (std::ispunct(c) && c != '_') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}


void translateToBytecode() {
    const uint16_t BUFFER_SIZE = 4096;

    std::ifstream file(sourcecode_filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::remove((sourcecode_filepath.substr(0, sourcecode_filepath.size() - 2) + "ef").c_str());

    std::string buffer;
    buffer.reserve(BUFFER_SIZE);
    std::string statement;
    std::vector<std::string> tokens;
    
    char chunk[BUFFER_SIZE];
    while (file.read(chunk, sizeof(chunk)) || file.gcount() > 0) {
        buffer.append(chunk, file.gcount());

        size_t semicolonPos;
        while ((semicolonPos = buffer.find(';')) != std::string::npos) {

            statement = buffer.substr(0, semicolonPos);
            line = statement.find('\n') + 2;
            tokens = tokenizeStatement(statement);
            translateLine(tokens);

            buffer.erase(0, semicolonPos + 1);
        }
    }

    file.close();
}

#endif