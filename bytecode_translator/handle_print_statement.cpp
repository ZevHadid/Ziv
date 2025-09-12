#include "handle_print_statement.hpp"
#include "globals.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <variant>
#include "../helper/helper.hpp"

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
