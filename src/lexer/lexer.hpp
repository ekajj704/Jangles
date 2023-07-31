#ifndef LEXER_H_
#define LEXER_H_

#include "../main.hpp"
#include <fstream>

Token gettok();
void SetLine(std::string nextLine);

#endif