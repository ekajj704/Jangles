#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cstring>
#include <math.h>
using namespace std;

typedef struct Token{
    int tok;
    string val;
} Token;

enum TOKEN_TYPE{
    TOK_END_PARENTHESIS = 9999,
    TOK_INCREMENT = 35,
    TOK_DECREMENT = 34,
    TOK_POWER = 32,
    TOK_DIVIDE = 31,
    TOK_MULTIPLY = 30,
    TOK_MINUS = 29,
    TOK_PLUS = 28,
    TOK_DIVIDE_ASSIGN = 27,
    TOK_MULTIPLY_ASSIGN = 26,
    TOK_SUB_ASSIGN = 25,
    TOK_PLUS_ASSIGN = 24,
    TOK_ASSIGN = 23,
    TOK_COMMA = 22,
    TOK_LESS_THAN = 19,
    TOK_GREATER_THAN = 18,
    TOK_LESS_OR_EQUAL = 17,
    TOK_GREATER_OR_EQUAL = 16,
    TOK_EQUALS = 15,
    TOK_NOT_EQUAL = 14,
    TOK_OR = 13,
    TOK_AND = 12,
    TOK_NOT = 11,
    TOK_START_PARENTHESIS = 10,
    TOK_DOT = 9,
    TOK_WHILE = 7,
    TOK_IF = 6,
    TOK_RETURN = 5,
    TOK_IDENTIFIER = -1,
    TOK_FN = -2,
    TOK_LET = -3,
    TOK_EOF = -4,
    TOK_NUMBER = -5,
    TOK_STRING = -6,
    TOK_ENDLINE = -7,
    TOK_INSTRUCTION = -8,
    TOK_FUNCTIONCALL = -9,
    TOK_THEN = -10,
    TOK_TAB = -11,
    TOK_BOOL = -12,
};

const map<int, string> TOKEN_TYPE_NAMES = {
    {-1, "<Identifier>"},
    {-2, "<Fn>"}, 
    {-3, "<Let>"}, 
    {-4, "<EndOfFile>"}, 
    {-5, "<Number>"}, 
    {-6, "<String>"}, 
    {-7, "<EndOfLine>"}, 
    {-8, "<InterpreterInstruction>"},
    };

const map<int, int> opPrecedent = {
    {TOK_RETURN, -1},
    {'(', 0},
    {',', 5},
    {'=', 10},
    {'+', 20}, 
    {'-', 30},
    {'*', 40}, 
    {'/', 50}, 
    {'^', 60}
    };


#endif