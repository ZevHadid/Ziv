#include "translate_line.hpp"
#include "handle_function_declaration.hpp"
#include "handle_variable_declaration.hpp"
#include "handle_print_statement.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

void translateLine(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return;
    }

    if (tokens[0] == "func") {
        handleFunctionDeclaration(tokens);
    } else if (tokens.size() > 2 && tokens[0] == "var" && tokens[2] == "=") {
        handleVariableDeclaration(tokens);
    } else if (tokens[0] == "print") {
        handlePrintStatement(tokens);
    } else {
        std::cout << "Invalid syntax. What even is that?";
        std::exit(0);
    }
}
