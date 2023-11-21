#ifndef EXEC_H
#define EXEC_H

#include <string>
#include "function.hpp"

using namespace std;

int ExecFunction(Function* func);
int ExecFunction(Function* func, vector<Token>* args);
vector<Token> ExecOp(vector<Token>* expr);


#endif