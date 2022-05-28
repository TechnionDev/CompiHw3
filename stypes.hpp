#ifndef STYPES_H_
#define STYPES_H_
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

namespace hw3 {
typedef enum {
    STStatements,
    STStatement,
    STExpression,
    STId,
    STCall,
    STString
} SymbolType;

const string &verifyAllTypeNames(const string &type);
const string &verifyValTypeName(const string &type);
const string &verifyRetTypeName(const string &type);
const string &verifyVarTypeName(const string &type);

class STypeC {
    SymbolType symType;

   public:
    STypeC(SymbolType symType);
};
// TODO: 
typedef shared_ptr<STypeC> STypePtr;
// typedef STypeC *STypePtr;

class RetTypeNameC : public STypeC {
    string type;

   public:
    const string &getTypeName() const;
    RetTypeNameC(const string &type);
};

class VarTypeNameC : public RetTypeNameC {
   public:
    VarTypeNameC(const string &type);
};

class ExpC : public STypeC {
    string type;

   public:
    ExpC(const string &type);
    const string &getType() const;
    bool isInt() const;
    bool isBool() const;
    bool isString() const;
    bool isByte() const;
};

class IdC : public STypeC {
    string name;
    string type;

   public:
    IdC(const string &varName, const string &type);
    const string &getName() const;
    virtual const string &getType() const;
};

class FuncIdC : public IdC {
    vector<string> argTypes;
    string retType;

   public:
    FuncIdC(const string &name, const string &type, const vector<string> &argTypes);
    const vector<string> &getArgTypes() const;
    const string &getType() const;
};

class SymbolTable {
    map<string, IdC *> symTbl;
    vector<int> scopeStartOffsets;
    vector<vector<string> > scopeSymbols;
    int currOffset;

   public:
    SymbolTable();
    void addScope();
    void removeScope();
    void addSymbol(string name, IdC *type);
    IdC *getSymbol(const string &name);
    void printSymbolTable();
};

class CallC : public STypeC {
    string type;
    string symbol;

   public:
    CallC(const string &type, const string &symbol);
    const string &getType() const;
};

class StringC : public STypeC {
    string value;

   public:
    StringC(const char *str);
    const string& getString() const;
};



}  // namespace hw3

#define YYSTYPE hw3::STypePtr
#define NEW(x, y) (std::shared_ptr<hw3::x>(new hw3::x(y)))
#define STYPE_TO_STR(x) (dynamic_pointer_cast<StringC>(x)->getString())
#endif