#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <fstream>
#include "../helper/helper.hpp"

extern std::vector<EntityType> variableStack;
extern std::vector<std::unordered_map<std::string, int>> variableScopes;
extern std::vector<int> framePointers;

void storeEntity(const std::string& identifier, EntityType value);
void enterScope();
void exitScope();
EntityType lookUpVar(std::ifstream& inFile);
void reassignVar(const std::string& varName, EntityType rvalue);
