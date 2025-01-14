#ifndef BYTECODE_EXECUTOR_HPP
#define BYTECODE_EXECUTOR_HPP


#include <string>

#include "helper_header.hpp"
#include "memory_management.hpp"

void handleVariableDeclaration(std::ifstream& inFile) {
    std::string varName;
    rvalue_type datatype;
    EntityType value;

    readString(inFile, varName);
    inFile.read(reinterpret_cast<char*>(&datatype), sizeof(datatype));

    switch (datatype) {
        case rvalue_type::STRING: {
            std::string stringValue;
            readString(inFile, stringValue);
            value = stringValue.substr(1, stringValue.length() - 2);
            break;
        }
        case rvalue_type::INTEGER: {
            int intValue;
            inFile.read(reinterpret_cast<char*>(&intValue), sizeof(intValue));
            value = intValue;
            break;
        }
        case rvalue_type::FLOAT: {
            float floatValue;
            inFile.read(reinterpret_cast<char*>(&floatValue), sizeof(floatValue));
            value = floatValue;
            break;
        }
        case rvalue_type::BOOLEAN: {
            bool boolValue;
            inFile.read(reinterpret_cast<char*>(&boolValue), sizeof(boolValue));
            value = boolValue;
            break;
        }
        case rvalue_type::INFERRED: {
            value = lookUpVar(inFile);

            if (std::holds_alternative<std::string>(value)) datatype = rvalue_type::STRING;
            else if (std::holds_alternative<int32_t>(value)) datatype = rvalue_type::INTEGER;
            else if (std::holds_alternative<float>(value)) datatype = rvalue_type::FLOAT;
            else if (std::holds_alternative<bool>(value)) datatype = rvalue_type::BOOLEAN;

            break;
        }
    }

    storeEntity(varName, value);  // Store the variable in the current scope
}

void handlePrintInstruction(std::ifstream& inFile) {
    std::visit([&](auto&& v) { 
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, bool>) { 
            std::cout << (v ? "true" : "false"); 
        } else { 
            std::cout << v; 
        }
    }, lookUpVar(inFile));
}

void handleConcatInstruction(std::ifstream& inFile) {
    rvalue_type datatype;
    std::string rvalue;

    inFile.read(reinterpret_cast<char*>(&datatype), sizeof(datatype));

    if (datatype == rvalue_type::STRING) {
        readString(inFile, rvalue);
        std::get<std::string>(variableStack.back()) += rvalue.substr(1, rvalue.length() - 2);
    } else if (datatype == rvalue_type::INFERRED) {
        std::get<std::string>(variableStack.back()) += std::get<std::string>(lookUpVar(inFile));
    }
}

void executeBytecode() {
    enterScope(); // enter global scope

    instruction_type instructionType;


    std::ifstream inFile(sourcecode_filepath.substr(0, sourcecode_filepath.size() - 2) + "ef", std::ios::binary);


    while (inFile.read(reinterpret_cast<char*>(&instructionType), sizeof(instructionType))) {
        switch (instructionType) {
            case instruction_type::VARIABLE:
                handleVariableDeclaration(inFile);
            break;

            case instruction_type::PRINT:
                handlePrintInstruction(inFile);
            break;

            case instruction_type::CONCAT:
                handleConcatInstruction(inFile);
            break;

            default:
                std::cout << "unknown instruction" << std::endl;
            break;
        }
    }


    inFile.close();
}

#endif