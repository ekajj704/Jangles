#include "../headers/lexer.hpp"
#include "../headers/function.hpp"
#include "../headers/exec.hpp"
#include "../headers/debugprint.hpp"
#include <fstream>

map<string, Function> functionDictionary = {{"top", Function()}};
vector<string> tabLevelFunctions = {"top"};

int tabLevel;
Token curTok;

void ParseArg(Function* fn){
    
    if(curTok.tok == TOK_COMMA){
        curTok = NextToken();
    }

    if(curTok.tok != TOK_IDENTIFIER){
        PrintError("Expected identifier, instead got: " + curTok.val);
        exit(1);
    }

    fn->AddArg(curTok.val);

    curTok = NextToken();

    if(curTok.tok != TOK_COMMA && curTok.tok != TOK_END_PARENTHESIS){
        PrintError("Unexpected token: " + curTok.val);
    }
}

void LowerTabLevel(int tabs){
    if(tabs == 0 && tabLevelFunctions.size() > 1){
        tabLevelFunctions.pop_back();
        tabs--;
    }
}

void ParsePrototype(){
    curTok = NextToken();
    Function fn;

    if(curTok.tok != TOK_FUNCTIONCALL){
        PrintError("Expected function name, instead got: " + curTok.val);
        exit(1);
    }

    fn.SetName(curTok.val);
    curTok = NextToken();
    if(curTok.tok != TOK_START_PARENTHESIS){
        PrintError("Expected '(', instead got: " + curTok.val);
        exit(1);
    }

    curTok = NextToken();

    if(curTok.tok == TOK_END_PARENTHESIS){
        functionDictionary.insert({fn.GetName(), fn});
        tabLevelFunctions.push_back(fn.GetName());
        curTok = NextToken();
        return;
    }
    
    while(curTok.tok != TOK_END_PARENTHESIS){
        ParseArg(&fn);
    }

    functionDictionary.insert({fn.GetName(), fn});
    tabLevelFunctions.push_back(fn.GetName());
    curTok = NextToken();

}

void ParseLine(){
    vector<Token> line;
    int tabCount = 0;

    if(curTok.tok == TOK_FN){
        ParsePrototype();
        return;
    }

    while(curTok.tok != TOK_ENDLINE && curTok.tok != TOK_EOF){
        line.push_back(curTok);
        curTok = NextToken();
    }
    curTok = NextToken();
   

    for(size_t i = 0; i < line.size(); i++){
        if(line.at(i).tok == TOK_TAB){
            tabCount++;
        }
    }

    if(!line.empty()){
        LowerTabLevel(tabCount);
        functionDictionary.at(tabLevelFunctions.back()).AddLine(&line);
    }
}

void CallFunction(string name){
    if(functionDictionary.count(name) == 0){
        PrintError("Use of undeclared function: " + name);
        exit(1);
    }
    ExecFunction(&functionDictionary.at(name));
}

void CallFunction(string name, vector<Token> *args){
    if(functionDictionary.count(name) == 0){
        PrintError("Use of undeclared function: " + name);
        exit(1);
    }
    ExecFunction(&functionDictionary.at(name), args);
}

void ParseFile(FILE* fptr){
    SetFile(fptr);

    curTok = NextToken();
    while(curTok.tok != TOK_EOF){
        ParseLine();
    }

    CallFunction("top");
}