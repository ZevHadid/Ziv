#include "bytecode_executor.hpp"
#include "handle_variable_declaration.hpp"
#include "handle_print_instruction.hpp"
#include "handle_concat_instruction.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "../helper/helper.hpp"
#include "../memory/memory_management.hpp"

void executeBytecode() {
    enterScope();

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
