#include "stypes.hpp"

#include "hw3_output.hpp"

using namespace output;

namespace hw3 {

STypeC::STypeC(SymbolType symType) : symType(symType) {}

const string &verifyAllTypeNames(const string &type) {
    if (type == "INT" or type == "BOOL" or type == "BYTE" or type == "VOID" or type == "STRING" or type == "BAD_VIRTUAL_CALL") {
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
    if (verifyAllTypeNames(type) != "VOID") {
        return type;
    } else {
        errorMismatch(-1);
        exit(1);
    }
}

RetTypeNameC::RetTypeNameC(const string &type) : STypeC(STRetType), type(verifyRetTypeName(type)) {}

VarTypeNameC::VarTypeNameC(const string &type) : RetTypeNameC(verifyVarTypeName(type)) {}

ExpC::ExpC(const string &type) : STypeC(STExpression), type(verifyValTypeName(type)) {}

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
vector<string> &FuncIdC::getArgTypes() {
    return this->argTypes;
}

SymbolTable::SymbolTable() {
    this->nestedLoopDepth = 0;
    this->currOffset = 0;
    this->addScope();
    this->addSymbol("print", NEW(FuncIdC, ("print", "VOID", vector<string>({"STRING"}))));
    this->addSymbol("printi", NEW(FuncIdC, ("printi", "VOID", vector<string>({"INT"}))));
}

void SymbolTable::addScope(int funcArgCount) {
    if (not((funcArgCount >= 0 and this->scopeStartOffsets.size() == 1) or (this->scopeStartOffsets.size() > 1 and funcArgCount == 0) or this->scopeStartOffsets.size() == 0)) {
        throw "Code error. We should only add a scope of a function when we are in the global scope";
    }
    if (funcArgCount != 0) {
        this->currOffset -= funcArgCount;
    }
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

const string &RetTypeNameC::getTypeName() const {
    return this->type;
}

void SymbolTable::addSymbol(string name, shared_ptr<IdC> type) {
    // Check that the symbol doesn't exist in the scope yet
    if (type == nullptr) {
        throw "Can't add a nullptr to the symbol table";
    }
    if (this->scopeSymbols.back().end() != find(this->scopeSymbols.back().begin(), this->scopeSymbols.back().end(), name)) {
        errorDef(yylineno, name);
    }
    this->scopeSymbols.back().push_back(name);
    this->symTbl[name] = type;

    if (this->scopeStartOffsets.size() > 1) {
        this->currOffset++;
    }
}

shared_ptr<IdC> SymbolTable::getVarSymbol(const string &name) {
    auto symbol = this->symTbl[name];

    // Check that the symbol exists in the symbol table
    if (symbol == nullptr or DC(FuncIdC, symbol) != nullptr) {
        errorUndef(yylineno, name);
    }

    return symbol;
}

shared_ptr<FuncIdC> SymbolTable::getFuncSymbol(const string &name, bool shouldError) {
    auto symbol = this->symTbl[name];

    // Check that the symbol exists in the symbol table
    shared_ptr<FuncIdC> funcSym = nullptr;

    if ((symbol == nullptr or (funcSym = DC(FuncIdC, symbol)) == nullptr) and shouldError) {
        errorUndefFunc(yylineno, name);
    }

    return funcSym;
}

void SymbolTable::printSymbolTable() {
    int offset = 0;
    for (auto it = this->symTbl.begin(); it != this->symTbl.end(); ++it) {
        printID(it->first, offset++, it->second->getType());
    }
}

StringC::StringC(const char *str) : STypeC(STString), value(str) {}

const string &StringC::getString() const {
    return this->value;
}

}  // namespace hw3
