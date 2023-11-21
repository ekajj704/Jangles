#include <vector>
#include <string>
#include <iostream>
#include "../headers/main.hpp"

using namespace std;

void PrintError(string error){
    fprintf(stderr, "ERROR: %s\n", error.c_str());
}

void PrintVectorDebug(vector<Token> *vec){
    for (size_t i = 0; i < vec->size(); i++)
    {
        fprintf(stdout, "%d :: %s\n", vec->at(i).tok, vec->at(i).val.c_str());
    } 
}

void PrintDebug(string message){
    fprintf(stderr, "DEBUG: %s\n", message.c_str());
}