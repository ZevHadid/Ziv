#include "bytecode_translator/bytecode_translator.hpp"
#include "bytecode_executor/bytecode_executor.hpp"
#include "helper/helper.hpp"
#include <string>

int main(int argc, char* argv[]) {

    sourcecode_filepath = "main.ziv";
    translateToBytecode();
    executeBytecode();

    std::string cmd;

    for (int i = 1; i < argc; ++i) {
        sourcecode_filepath = argv[2];
        cmd = argv[1];
    }

    if (cmd == "exec") {    
        translateToBytecode();
        executeBytecode();
    } else if (cmd == "parse") {
        translateToBytecode();
    }

    return 0;
}   