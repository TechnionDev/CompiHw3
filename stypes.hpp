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

class STypeC {
};
typedef shared_ptr<STypeC *> STypePtr;

class VarTypeNameC {
    string type;
};

class ExpC {

};

class ValueC {
};

class IdC : public STypeC {
    string name;

   public:
    IdC(const string &varName);
};

class StringC : public ValueC {
    string value;

   public:
    StringC(const string &value);
};

class ByteC : public ValueC {
    char value;

   public:
    ByteC(const char &value);
};

class BoolC : public ValueC {
    bool value;

   public:
    BoolC(const bool &value);
};

class IntC : public ValueC {
    int value;

   public:
    IntC(const int &value);
};

class SymbolTable {
    map<string, STypePtr > symTbl;
    vector<set<string> > scopeSymbols;

   public:
    void addScope();
    void removeScope();
    void addSymbol(string name, STypePtr type);
    STypePtr getSymbol(string name);
    void printSymbolTable();
};
}  // namespace hw3

#endif