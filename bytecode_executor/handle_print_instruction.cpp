#include "handle_print_instruction.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <type_traits>
#include "../memory/memory_management.hpp"

void handlePrintInstruction(std::ifstream& inFile) {
    std::visit([&](auto&& v) {
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, bool>) {
            std::cout << (v ? "true" : "false");
        } else {
            std::cout << v;
        }
    }, lookUpVar(inFile));
}
