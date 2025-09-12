#include "handle_function_declaration.hpp"
#include <vector>
#include <string>

void handleFunctionDeclaration(std::vector<std::string>& tokens) {
    size_t leftParenthesisPos = 0;
    size_t rightParenthesisPos = 0;
    size_t leftCurlyBracePos = 0;
    size_t rightCurlyBracePos = 0;

    // Find the position of the opening parenthesis
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "(") {
            leftParenthesisPos = i;
            break;
        }
    }

    // Find the position of the closing parenthesis
    int parenthesisCount = 1;
    for (size_t i = leftParenthesisPos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "(") {
            parenthesisCount++;
        } else if (tokens[i] == ")") {
            parenthesisCount--;
            if (parenthesisCount == 0) {
                rightParenthesisPos = i;
                break;
            }
        }
    }

    // Find the position of the opening curly brace
    for (size_t i = rightParenthesisPos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "{") {
            leftCurlyBracePos = i;
            break;
        }
    }

    // Find the position of the closing curly brace
    int curlyBraceCount = 1;
    for (size_t i = leftCurlyBracePos + 1; i < tokens.size(); ++i) {
        if (tokens[i] == "{") {
            curlyBraceCount++;
        } else if (tokens[i] == "}") {
            curlyBraceCount--;
            if (curlyBraceCount == 0) {
                rightCurlyBracePos = i;
                break;
            }
        }
    }

    // TODO: use position variables to extract the function declaration from the possible variable declaration

}
