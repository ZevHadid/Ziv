#include "bytecode_translator.hpp"
#include "globals.hpp"
#include "tokenize_statement.hpp"
#include "translate_line.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>

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
