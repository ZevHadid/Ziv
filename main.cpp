#include "bytecode_translator.hpp"
#include "bytecode_executor.hpp"

int main(int argc, char* argv[]) {
    std::cin >> sourcecode_filepath;
    translateToBytecode();
    executeBytecode();

    return 0;
}   