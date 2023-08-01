#include <iostream>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>


void compile(std::string name){
    std::string oCommand = "g++ -Wall -c " + name + ".cpp";
    std::string cCommand = "g++ " + name + ".cpp  -lmingw32 -lSDL2main -lSDL2 -o " + name + ".exe";
    std::string del = "del " + name + ".cpp " + name + ".hpp " + name + ".o";

    system(oCommand.c_str());
    system(cCommand.c_str());
    //system(del.c_str());
    

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