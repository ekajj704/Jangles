#include "../headers/function.hpp"
#include "../headers/debugprint.hpp"

string Function::GetName(){
    return this->name;
}

void Function::SetName(string name){
    this->name = name;
}

void Function::AddArg(string arg){
    this->args.push_back(arg);
}

void Function::AddLine(vector<Token>* line)
{
    this->lines.push_back(*line);
}

vector<Token> Function::GetLine(size_t index){
    return this->lines.at(index);
}

string Function::GetArg(size_t index){
    return this->args.at(index);
}

int Function::LineCount(){
    return this->lines.size();
}

int Function::ArgCount(){
    return this->args.size();
}