#ifndef PARSER_H_
#define PARSER_H_

#include <fstream>

int Parse(std::ifstream *srcfptr, std::string name);
int isEOF();

#endif