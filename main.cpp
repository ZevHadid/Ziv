#include "bytecode_translator.hpp"
#include "bytecode_executor.hpp"

int main() {
    std::cin >> sourcecode_filepath;
    translateToBytecode();
    executeBytecode();

    return 0;
}