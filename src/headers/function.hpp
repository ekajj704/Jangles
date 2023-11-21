#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "main.hpp"

class Function{
    string name;
    vector<string> args;
    vector<vector<Token>> lines;

    public:
    ~Function() = default;

    string GetName();
    void SetName(string name);
    void AddArg(string arg);
    void AddLine(vector<Token>* line);
    vector<Token> GetLine(size_t index);
    string GetArg(size_t index);
    int LineCount();
    int ArgCount();
};

#endif