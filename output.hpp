#ifndef _236360_2_
#define _236360_2_

#include <string>

namespace output {
extern const std::string rules[];
void printProductionRule(const int ruleno);
void printProductionRule(const int ruleno, const char* val);
void errorLex(const int lineno);
void errorSyn(const int lineno);
int yyerror(const char* s);
};  // namespace output

#endif
