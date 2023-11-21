#ifndef CONSOLE_H_
#define CONSOLE_H_

using namespace std;

#include "../book.hpp"
#include "../../headers/exec.hpp"

vector<Token> Print(vector<Token>* args){
    if(args->size() == 1){
        CheckVar(args);
        cout << args->front().val << '\n';
    }
    
    return vector<Token>({Token({0, ""})});
}

vector<Token> Read(){
    string input;
    cin >> input;
    return vector<Token>({Token({TOK_STRING, input})});
}

set<string> fnNames = set<string>({
    "Print",
    "Read",
});

class console : public Book{

    public:
        console() : Book("Console", fnNames){}

        vector<Token> RunFunction(string fnName, vector<Token>* args) override{

            if(fnName == "Print"){
                return Print(args);
            }

            if(fnName == "Read"){
                return Read();
            }

            return vector<Token>();
        }
};

#endif