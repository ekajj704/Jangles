#ifndef BOOK_H_
#define BOOK_H_

#include "../headers/main.hpp"
#include "librarian.hpp"
#include <set>

using namespace std;

class Book
{
protected:
    set<string> functions;
    string bookName;

public:
    Book(string name, set<string> fnNames): bookName(name), functions(fnNames) {}
    ~Book() = default;

    bool HasFunction(string fn){
        if(functions.count(fn)){
            return true;
        }
        return false;
    }

    void SetFunctions(set<string> fns){
        functions = fns;
    }

    string Name() { return bookName; }

    virtual vector<Token> RunFunction(string fnName, vector<Token>* args) = 0;
};


#endif