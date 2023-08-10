#include "../lexer/lexer.hpp"
#include "../filewriter/filewriter.hpp"
#include <string>
#include <vector> 
#include <iostream>
#include "../main.hpp"
#include <fstream>
#include "Node.hpp"
#include <map>


Token CurToken;

size_t CurLevel;
std::vector<std::unique_ptr<BlockExpr>> LevelOwners;

std::vector<std::string> FunctionPrototypes;

std::ifstream *srcfptr;

char CurLine[200];

int isEOF(){
    return srcfptr->eof();
}

void GetNextToken(){
    CurToken = gettok();
}

void EatLevel(size_t LineLevel){
    if(LineLevel < CurLevel){
        std::string blockString = LevelOwners[CurLevel]->CodeGen();
        LevelOwners.pop_back();
        CurLevel--;
        LevelOwners[CurLevel]->AddLine(blockString);
    }
}

std::string ParseConditional(){   
    std::string Name = CurToken.StringVal;
    std::string Condition = "";

    GetNextToken();
    while(CurToken.Tok != tok_eof && CurToken.Tok != tok_endLine){
        Condition += CurToken.StringVal;
        GetNextToken();
    }
    return Name + "(" + Condition + ")";
}

ArgumentExpr ParseArgument(){
    std::string Name;
    std::string Type = "";

    while(CurToken.Tok != tok_eof && CurToken.Tok != tok_endLine && CurToken.StringVal != "=" && CurToken.StringVal != ":" && CurToken.StringVal != ")"){
        Name += CurToken.StringVal;
        GetNextToken();
    }
    
    if(CurToken.StringVal != ":"){
        Type = "auto";
        return ArgumentExpr(Name, Type);
    }

    GetNextToken();
    while(CurToken.Tok != tok_eof && CurToken.Tok != tok_endLine && CurToken.StringVal != "=" && CurToken.StringVal != "," && CurToken.StringVal != ")"){
        Type += CurToken.StringVal;
        GetNextToken();
    }

    if(CurToken.StringVal == ",")
        GetNextToken();
        
    
    return ArgumentExpr(Name, Type); 
}

std::string ParseFunction(){
    auto Fn = std::make_unique<FunctionExpr>();

    GetNextToken();
    if(CurToken.Tok != tok_identifier){
        fprintf(stderr, "Error: Expected function name\n");
        return "";
    }

    Fn->Name = CurToken.StringVal;

    GetNextToken();

    if(CurToken.StringVal != "("){
        fprintf(stderr, "Error: Expected '(', instead got %s\n", CurToken.StringVal.c_str());
        return "";
    }
        
        GetNextToken();
    while(CurToken.StringVal != ")"){
        ArgumentExpr Arg = ParseArgument();
        if(Arg.Type == "auto"){
            fprintf(stderr, "Error: argument %s in function prototype %s requires a type\n", Arg.Name.c_str(), Fn->Name.c_str());
            return "";
        }

        Fn->Args += Arg.CodeGen();

        if(CurToken.Tok == tok_endLine || CurToken.Tok == tok_eof){
            fprintf(stderr, "Error: Expected ')' in function prototype %s\n", Fn->Name.c_str());
            return "";
        }
    }
    
    if(!Fn->Args.empty())
        Fn->Args.pop_back();

    GetNextToken();
    if(CurToken.StringVal != "->"){
        Fn->ReturnType = "void";
        
        if(CurToken.Tok != tok_endLine && CurToken.Tok != tok_eof){
            fprintf(stderr, "Error: Unknown token in function declaration %s\n", CurToken.StringVal.c_str());
            return "";
        }

        return Fn->CodeGen();
    }

    
    GetNextToken();
    while(CurToken.Tok != tok_endLine && CurToken.Tok != tok_eof){
        Fn->ReturnType += CurToken.StringVal;
        GetNextToken();
    }
    return Fn->CodeGen();
}

ReturnExpr ParseReturn(){
    std::string content;
    GetNextToken();

    while(CurToken.Tok != tok_endLine && CurToken.Tok != tok_eof){
        content += CurToken.StringVal;
        GetNextToken();
    }

    return ReturnExpr(content);
}

VariableExpr ParseLet(){
    GetNextToken();
    ArgumentExpr Arg =  ParseArgument();

    std::string Val = "";

    
    
    if(CurToken.Tok == tok_endLine || CurToken.Tok == tok_eof){
        return VariableExpr(Arg, Val);
    }

    if(CurToken.StringVal != "="){
        fprintf(stderr, "Error: Expected '='. Instead got %s\n", CurToken.StringVal.c_str());
    }
    GetNextToken();
    while(CurToken.Tok != tok_endLine && CurToken.Tok != tok_eof){
        Val += CurToken.StringVal;
        GetNextToken();
    }
    GetNextToken();

    return VariableExpr(Arg, Val);
}

void ParseImport(){
    GetNextToken();
    if(CurToken.StringVal == "SDL"){
        setflag(SDL_FLAG);
        GetNextToken();
        return;
    }

    fprintf(stderr, "Error: unrecognized dependency \"%s\"\n", CurToken.StringVal.c_str());
    GetNextToken();
}

int ParseLine(){
    
    size_t LineLevel = 0;
    GetNextToken();
    while (CurToken.Tok == tok_tab){
        LineLevel++;
        GetNextToken();
    }

    if(CurToken.Tok == tok_eof || CurToken.Tok == tok_endLine){
        return 0;
    }

    while (LineLevel < CurLevel)
    {
        EatLevel(LineLevel);
    }

    if(LineLevel > LevelOwners.size() - 1)
        LineLevel = LevelOwners.size() - 1;

    if(CurToken.Tok == tok_return){
        ReturnExpr ret = ParseReturn();
        LevelOwners[LineLevel]->AddLine(ret.CodeGen());
        return 1;
    }

    if(CurToken.Tok == tok_let){
        //printf("%d :: %d\n", LineLevel, CurLevel);
        VariableExpr Var = ParseLet();
        LevelOwners[LineLevel]->AddLine(Var.code_gen_let());
        return 1;
    }

    if(CurToken.Tok == tok_const){
        VariableExpr Var = ParseLet();
        LevelOwners[LineLevel]->AddLine(Var.code_gen_const());
        return 1;
    }

    if(CurToken.Tok == tok_conditional){
        auto ConBlock = std::make_unique<BlockExpr>();
        ConBlock->Prototype = ParseConditional();
        LevelOwners.push_back(std::move(ConBlock));
        CurLevel++;
        return 1;
    }

    if(CurToken.Tok == tok_function){
        auto FnBlock = std::make_unique<BlockExpr>();
        std::string prototype = ParseFunction();
        FnBlock->Prototype = prototype;
        LevelOwners.push_back(std::move(FnBlock));
        CurLevel++;
        FunctionPrototypes.push_back(prototype);
        return 1;
    }

    if(CurToken.Tok == tok_table){
        auto TableBlock = std::make_unique<BlockExpr>();
    }

    if(CurToken.Tok == tok_import){
        ParseImport();
    }


    if(CurToken.Tok == tok_endLine){
        return 1;
    }


    std::string line;
    while (CurToken.Tok != tok_endLine && CurToken.Tok != tok_eof)
    {   
        line += CurToken.StringVal;
        GetNextToken();
    }
    line += ";\n";
    
    LevelOwners[LineLevel]->AddLine(line);

    return 1;
}

int Parse(std::ifstream *aSrcfptr, std::string name){
    srcfptr = aSrcfptr;

    LevelOwners.push_back(std::make_unique<BlockExpr>());

    while(true){
        srcfptr->getline(CurLine, 200);
        SetLine(CurLine);
        ParseLine();

        if(srcfptr->eof()){
            break;
        }
    }

    while (CurLevel > 0)
    {
        EatLevel(0);
    }

    std::string CBodyString = "";
    for(size_t i = 0; i < LevelOwners[0]->Body.size(); i++){
        CBodyString += LevelOwners[0]->Body[i];
    }

    std::string HBodyString = "";
    for(size_t i = 0; i < FunctionPrototypes.size(); i++){
        HBodyString += FunctionPrototypes[i] + ";\n";
    }

    writefiles(name, CBodyString, HBodyString);
    
    return 0;
}