#include "handle_concat_instruction.hpp"
#include <fstream>
#include <string>
#include <variant>
#include "../helper/helper.hpp"
#include "../memory/memory_management.hpp"

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
