#ifndef _MAIN_H
#define _MAIN_H
#include <string>

enum Token{
    tok_eof = -1,

    tok_func = -2,

    tok_define = -3,

    tok_identifier = -4,

    tok_number = -5,

};

static std::string IdentifierString;
static double NumVal;


#endif