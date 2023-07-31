#ifndef MAIN_H_
#define MAIN_H_
#include <string>

enum Tok{
    tok_function = -1,
    tok_import = -2,
    tok_identifier = -3,
    tok_let = -4,
    tok_const = -5,
    tok_table = -6,
    tok_literal = -7,
    tok_endLine = -8,
    tok_symbol = -11,
    tok_eof = -12,
    tok_conditional = -13,
    tok_return = -14,
    tok_tab = -16,
};

class Token{
    public:
        int Tok;
        std::string StringVal;
};

#endif