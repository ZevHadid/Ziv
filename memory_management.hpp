#ifndef MEMORY_MANAGEMENT_HPP
#define MEMORY_MANAGEMENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

using EntityType = std::variant<int, bool, float, std::string>;

std::vector<EntityType> variableStack;
std::vector<std::unordered_map<std::string, int>> variableScopes;
std::vector<int> framePointers;

void storeEntity(const std::string& identifier, EntityType value) {
    auto& currentScope = variableScopes.back();

    // Check if the variable already exists in the current scope
    if (currentScope.find(identifier) != currentScope.end()) {
        std::cout << "Error: Variable '" << identifier << " already declared in the current scope.\n";
        std::exit(0);
    }

    // Add the variable to the current scope and push to the stack
    int offset = variableStack.size();
    currentScope[identifier] = offset;
    variableStack.push_back(value);
}

void enterScope() {
    framePointers.push_back(variableStack.size());  // Record the current stack top
    variableScopes.push_back({});  // Create a new scope (empty map)
}

void exitScope() {
    if (!framePointers.empty()) {
        int frameStart = framePointers.back();
        variableStack.resize(frameStart);  // Remove all variables in the current frame
        framePointers.pop_back();
        variableScopes.pop_back();  // Remove the variable map for the current scope
    }
}

EntityType lookUpVar(std::ifstream& inFile) {
    EntityType value;
    std::string varName;
    readString(inFile, varName);

    bool found = false;
    for (int i = variableScopes.size() - 1; i >= 0; --i) {
        if (variableScopes[i].find(varName) != variableScopes[i].end()) {
            int offset = variableScopes[i][varName];    
            value = variableStack[offset];
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Error: Variable not found\n";
        std::exit(0);
    }

    return value;
}

#endif