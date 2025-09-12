#include "tokenize_statement.hpp"
#include <vector>
#include <string>
#include <cctype>

std::vector<std::string> tokenizeStatement(const std::string& statement) {
    std::vector<std::string> tokens;
    std::string token;
    bool insideQuotes = false;

    for (size_t i = 0; i < statement.length(); ++i) {
        char c = statement[i];

        if (c == '"') {
            insideQuotes = !insideQuotes;
            token += c;
        } else if (insideQuotes) {
            token += c;
        } else if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (std::ispunct(c) && c != '_') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}
