#include "memory_management.hpp"
#include <iostream>
#include <cstdlib>

std::vector<EntityType> variableStack;
std::vector<std::unordered_map<std::string, int>> variableScopes;
std::vector<int> framePointers;

void storeEntity(const std::string& identifier, EntityType value) {
    auto& currentScope = variableScopes.back();

    if (currentScope.find(identifier) != currentScope.end()) {
        std::cout << "Error: Variable '" << identifier << " already declared in the current scope.\n";
        std::exit(0);
    }

    int offset = variableStack.size();
    currentScope[identifier] = offset;
    variableStack.push_back(value);
}

void enterScope() {
    framePointers.push_back(variableStack.size());
    variableScopes.push_back({});
}

void exitScope() {
    if (!framePointers.empty()) {
        int frameStart = framePointers.back();
        variableStack.resize(frameStart);
        framePointers.pop_back();
        variableScopes.pop_back();
    }
}

EntityType lookUpVar(std::ifstream& inFile) {
    EntityType rvalue;
    std::string varName;
    readString(inFile, varName);

    bool found = false;
    for (int i = variableScopes.size() - 1; i >= 0; --i) {
        if (variableScopes[i].find(varName) != variableScopes[i].end()) {
            int offset = variableScopes[i][varName];    
            rvalue = variableStack[offset];
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Error: Variable not found\n";
        std::exit(0);
    }

    return rvalue;
}

void reassignVar(const std::string& varName, EntityType rvalue) {
    bool found = false;
    for (int i = variableScopes.size() - 1; i >= 0; --i) {
        if (variableScopes[i].find(varName) != variableScopes[i].end()) {
            int offset = variableScopes[i][varName];    
            variableStack[offset] = rvalue;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Error: Variable not found\n";
        std::exit(0);
    }
}
