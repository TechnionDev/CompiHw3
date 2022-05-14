#include "stypes.hpp"

namespace hw3 {

VarTypeIdC::VarTypeIdC(const string &varName) {
    this->name = varName;
}

StringC::StringC(const string &value) {
    this->value = value;
}

ByteC::ByteC(const char &value) {
    this->value = value;
}

BoolC::BoolC(const bool &value) {
    this->value = value;
}

IntC::IntC(const int &value) {
    this->value = value;
}

void SymbolTable::addScope() {
    this->scopeSymbols.push_back(set<string>());
}

void SymbolTable::removeScope() {
    // For each string in the last scope, remove it from the symbol table
    for (string s : this->scopeSymbols.back()) {
        this->symTbl.erase(s);
    }

    scopeSymbols.pop_back();
}

void SymbolTable::addSymbol(string name, STypePtr type) {
    // Check that the symbol doesn't exist in the scope yet
    if (this->scopeSymbols.back().find(name) != this->scopeSymbols.back().end()) {
        throw "Symbol already exists in current scope";
    }
    this->scopeSymbols.back().insert(name);
    this->symTbl[name] = type;
}

}  // namespace hw3
