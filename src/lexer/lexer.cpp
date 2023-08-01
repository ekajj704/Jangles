#include <iostream>
#include "../main.hpp"
#include "../parser/parser.hpp"

std::string line;
static size_t i = 0;
static int lastChar = ' ';
static Token newToken;

char GetChar(){
    if(i == line.length())
        return '\n';
    char Char = line.at(i);
    i++;
    //printf("%d\n", i -1);
    return Char;
}

void SetLine(std::string nextLine){
    lastChar = ' ';
    i = 0;
    line = nextLine;
    //printf("%s\n", line.c_str());
}

Token getTok(){    
    int spaces = 0;
    
    
    while(lastChar == ' '){
        lastChar = GetChar();
        
        spaces++;
        if(spaces == 4){
            newToken.Tok = tok_tab;
            return newToken;
        }
    }

    
    if(lastChar == '\n' && isEOF()){
        newToken.Tok = tok_eof;
        return newToken;
    }

    if(lastChar == '\n'){
        newToken.Tok = tok_endLine;
        return newToken;
    }
    
    if(isalpha(lastChar)){
        newToken.StringVal = lastChar;
        //printf("%d :: %s\n", i, newToken.StringVal.c_str());

        while(isalnum(lastChar = GetChar()))
            newToken.StringVal += lastChar;

        if(newToken.StringVal == "fn"){
            newToken.Tok = tok_function;
            return newToken;
        }
        
        if(newToken.StringVal == "import"){
            newToken.Tok = tok_import;
            return newToken;
        }

        if(newToken.StringVal == "let"){
            newToken.Tok = tok_let;
            return newToken;
        }

        if(newToken.StringVal == "const"){
            newToken.Tok = tok_const;
            return newToken;
        }

        if(newToken.StringVal == "table"){
            newToken.Tok = tok_table;
            return newToken;
        }

        if(newToken.StringVal == "return"){
            newToken.Tok = tok_return;
            return newToken;
        }

        if(newToken.StringVal == "if"){
            newToken.Tok = tok_conditional;
            return newToken;
        }

        if(newToken.StringVal == "while"){
            newToken.Tok = tok_conditional;
            return newToken;
        }
        
        newToken.Tok = tok_identifier;
        return newToken;
    }

    if(lastChar == '.' || isdigit(lastChar)){
        std::string numString;
        numString = lastChar;
        while (isdigit(lastChar = GetChar()) || lastChar == '.')
            numString += lastChar;
        
        newToken.StringVal = numString;
        newToken.Tok = tok_literal;
        return newToken;
    }

    

    newToken.StringVal = lastChar;

    if(lastChar == ','){
        newToken.Tok = tok_symbol;
        lastChar = GetChar();
        return newToken;
    }

    if(lastChar == '('){
        newToken.Tok = tok_symbol;
        lastChar = GetChar();
        return newToken;
    }

    if(lastChar == ')'){
        newToken.Tok = tok_symbol;
        lastChar = GetChar();
        return newToken;
    }

    if(lastChar == '\"'){
        newToken.Tok = tok_symbol;
        lastChar = GetChar();
        while(lastChar != '\"' && lastChar != '\n'){
            newToken.StringVal += lastChar;
            lastChar = GetChar();
        }
        if(lastChar != '\"'){
            fprintf(stderr, "Error: Expected closing '\"'\n");
            return newToken;
        }

        newToken.StringVal += lastChar;
        lastChar = GetChar();

        return newToken;
    }


    while(!isalnum(lastChar = GetChar()) && !isspace(lastChar) && lastChar != ',' && lastChar != '\"' && lastChar != ')' && lastChar != ':')
        newToken.StringVal += lastChar;
    
    if(newToken.StringVal == "//"){
        while (lastChar != EOF && lastChar != '\r' && lastChar != '\n')
            lastChar = GetChar();
        
        Token r = {1, ""};
        return r;
    }

    newToken.Tok = tok_symbol;
    return newToken;
    
    

}

Token gettok(){
    Token token = getTok();
    //printf("Token place: %s\n", token.StringVal.c_str());
    return token;
}