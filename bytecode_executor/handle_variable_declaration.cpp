#include "handle_variable_declaration.hpp"
#include <fstream>
#include <string>
#include <variant>
#include "../helper/helper.hpp"
#include "../memory/memory_management.hpp"

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

    storeEntity(varName, value);
}
