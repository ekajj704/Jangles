#ifndef PARSER_H_
#define PARSER_H_

#include "main.hpp"

void ParseFile(FILE* fptr);
void CallFunction(string name);
void CallFunction(string name, vector<Token> *args);

#endif