#include "stypes.hpp"

#include "hw3_output.hpp"

using namespace output;

namespace hw3 {

STypeC::STypeC(SymbolType symType) : symType(symType) {}

const string &verifyAllTypeNames(const string &type) {
    if (type == "INT" or type == "BOOL" or type == "BYTE" or type == "VOID" or type == "STRING" or "BAD_VIRTUAL_CALL") {
        return type;
    } else {
        errorMismatch(-1);
        exit(1);
    }
}

const string &verifyValTypeName(const string &type) {
    if (verifyAllTypeNames(type) != "VOID") {
        return type;
    } else {
        errorMismatch(-1);
        exit(1);
    }
}

const string &verifyRetTypeName(const string &type) {
    if (verifyAllTypeNames(type) != "STRING") {
        return type;
    } else {
        errorMismatch(-1);
        exit(1);
    }
}

const string &verifyVarTypeName(const string &type) {
    if (verifyVarTypeName(type) != "VOID") {
        return type;
    } else {
        errorMismatch(-1);
        exit(1);
    }
}

RetTypeNameC::RetTypeNameC(const string &type) : type(verifyRetTypeName(type)) {}

VarTypeNameC::VarTypeNameC(const string &type) : RetTypeNameC(verifyVarTypeName(type)) {}

ExpC::ExpC(const string &type) : type(verifyValTypeName(type)) {}

CallC::CallC(const string &type, const string &symbol) : STypeC(STCall), type(verifyRetTypeName(type)), symbol(symbol) {}

bool ExpC::isInt() const {
    return this->type == "INT";
}

bool ExpC::isBool() const {
    return this->type == "BOOL";
}

bool ExpC::isString() const {
    return this->type == "STRING";
}

bool ExpC::isByte() const {
    return this->type == "BYTE";
}

const string &ExpC::getType() const { return type; }

IdC::IdC(const string &varName, const string &type) : STypeC(STId), name(varName), type(verifyVarTypeName(type)) {}

const string &IdC::getName() const {
    return this->name;
}

const string &IdC::getType() const {
    return this->type;
}

FuncIdC::FuncIdC(const string &name, const string &type, const vector<string> &argTypes) : IdC(name, "BAD_VIRTUAL_CALL"), argTypes(argTypes) {
    this->retType = verifyRetTypeName(type);
}

const string &FuncIdC::getType() const {
    return this->retType;
}

const vector<string> &FuncIdC::getArgTypes() const {
    return this->argTypes;
}

SymbolTable::SymbolTable() {
    this->addScope();
    this->addSymbol("print", NEW(IdC, ("print", "STRING")));
    this->addSymbol("print", NEW(IdC, ("printi", "int")));
}

void SymbolTable::addScope() {
    this->scopeSymbols.push_back(vector<string>());
    this->scopeStartOffsets.push_back(this->currOffset);
}

void SymbolTable::removeScope() {
    endScope();
    // For each string in the last scope, remove it from the symbol table
    int offset = this->scopeStartOffsets.back();
    this->currOffset -= this->scopeSymbols.size();
    for (string s : this->scopeSymbols.back()) {
        printID(s, offset++, this->symTbl[s]->getType());
        this->symTbl.erase(s);
    }

    scopeSymbols.pop_back();
    scopeStartOffsets.pop_back();
}

void SymbolTable::addSymbol(string name, shared_ptr<IdC> type) {
    // Check that the symbol doesn't exist in the scope yet
    if (this->scopeSymbols.back().find(name) != this->scopeSymbols.back().end()) {
        // TODO: errorDef
        throw "Symbol already exists in current scope";
    }
    this->scopeSymbols.back().push_back(name);
    this->symTbl[name] = type;
    this->currOffset++;
}

shared_ptr<IdC> SymbolTable::getSymbol(const string &name) {
    // Check that the symbol exists in the symbol table
    if (this->symTbl[name] == nullptr) {
        // TODO: errorUndef
        throw "Symbol does not exist in symbol table";
    }
    return this->symTbl[name];
}

void SymbolTable::printSymbolTable() {
    int offset = 0;
    for (auto it = this->symTbl.begin(); it != this->symTbl.end(); ++it) {
        printID(it->first, offset++, it->second->getTypeName());
    }
}

StringC::StringC(const char *str) : STypeC(STString), value(str) {}

const string &StringC::getString() const {
    return this->value;
}

}  // namespace hw3


