#include <iostream>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>
#include <windows.h>

int flags[1];

void setflag(int flag){
    flags[flag] = 1;
}

int getflag(int flag){
    return flags[flag];
}

void compile(std::string name){
    char buff[300];
    GetModuleFileName(NULL, buff, 300);
    
    std::string path = buff;
    path = path.substr(0, path.length() - 11);
    path += "MinGW\\bin\\g++";

    std::string SDLimport = "";

    if(getflag(SDL_FLAG))
        SDLimport = "-lmingw32 -lSDL2main -lSDL2";

    std::string oCommand = path + " -Wall -c " + name + ".cpp";
    std::string cCommand = path + " " + name + ".cpp " + SDLimport + " -o " + name + ".exe";
    std::string del = "del " + name + ".cpp " + name + ".hpp " + name + ".o";

    system(oCommand.c_str());
    system(cCommand.c_str());
    system(del.c_str());
    

}

int main(int argc, char** argv){
    std::string name = argv[1];
    std::string filename = name + ".jang";
    
    if(argc < 1)
        fprintf(stderr, "Error: No file specified\n");

    std::ifstream src;
    src.open(filename, std::ios::out);
    
    if(!src.is_open()){
        fprintf(stderr, "Error: failed to open file %s\n", filename.c_str());
        exit(-1);
    }
    
    Parse(&src, name);
    compile(name);
    src.close();
    exit(0);
}