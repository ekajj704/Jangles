#include <iostream>
#include "main.hpp"

static int gettok(){
    static int lastChar = ' ';

    //Checks if the last character was whitespace and furthers the pointer
    while(isspace(lastChar)){
        lastChar = getchar();
    }

    //checks if the last char was a letter, then gets the rest of the name
    if(isalpha(lastChar)){
        IdentifierString = lastChar;
        
        while (isalnum(lastChar = getchar()))
        {   
            IdentifierString += lastChar;
        }

        //checks for function declaration
        if(IdentifierString == "func"){
            return tok_func;
        }

        //checks for define command
        if(IdentifierString == "define"){
            return tok_define;
        }

        return tok_identifier;
        
    }

    //Handling numbers
    if(isdigit(lastChar) || lastChar == '.'){
        std::string NumStr;
        while(isdigit(lastChar) || lastChar == '.'){
            NumStr += lastChar;
            lastChar = getchar();
        }

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if(lastChar == '#'){
            while (lastChar != EOF && lastChar != '\n' && lastChar != '\r')
            {
                lastChar = getchar();
            }

            if(lastChar != EOF){
                return gettok();
            }
    }

    if(lastChar == EOF){
        return tok_eof;
    }

    int ThisChar = lastChar;
    lastChar = getchar();
    return ThisChar;
    
}