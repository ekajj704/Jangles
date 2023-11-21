#include "../headers/main.hpp"

static char lastChar;

static FILE* fptr;

void SetFile(FILE* newfptr){
    fptr = newfptr;
}


//Helper funciton for debugging
//Gets the next character
void GetChar(){
    lastChar = fgetc(fptr);
    //cout << lastChar << "\n";
}

Token GetTok(){
    Token newTok = Token();
    int spaceCounter = 0;

    if(lastChar == 0)
        GetChar();

    //Ignore whitespace
    while(isspace(lastChar) && lastChar != '\n'){
        spaceCounter++;
        
        if(lastChar == '\t' || spaceCounter == 4){
            newTok.tok = TOK_TAB;
            GetChar();
            return newTok;
        }
        
        GetChar();
    }

    if(lastChar == '\n'){
        newTok.tok = TOK_ENDLINE;
        GetChar();
        return newTok;
    }
    
    //Detect End of File
    if(lastChar == EOF || lastChar == 0){
        newTok.tok = TOK_EOF;
        return newTok;
    }

    //Detect numbers
    if(isdigit(lastChar) || lastChar == '.'){
        newTok.val = lastChar;
        GetChar();
        while(isdigit(lastChar) || lastChar == '.'){
            newTok.val += lastChar;
            GetChar();
        }
        newTok.tok = TOK_NUMBER;
        if(newTok.val == "."){
            newTok.tok = TOK_DOT;
        }
        return newTok;  
    }

    //identifiers
    if(isalpha(lastChar)){
        newTok.val = lastChar;
        GetChar();
        while(isalnum(lastChar)){
            newTok.val += lastChar;
            GetChar();
        }

        if(newTok.val == "fn"){
            newTok.tok = TOK_FN;
            return newTok;
        }

        if(newTok.val == "let"){
            newTok.tok = TOK_LET;
            return newTok;
        }

        if(newTok.val == "then"){
            newTok.tok = TOK_THEN;
            return newTok;
        }

        if(newTok.val == "return"){
            newTok.tok = TOK_RETURN;
            return newTok;
        }

        if(newTok.val == "while"){
            newTok.tok = TOK_WHILE;
            return newTok;
        }

        if(newTok.val == "if"){
            newTok.tok = TOK_IF;
            return newTok;
        }

        if(newTok.val == "true" || newTok.val == "false"){
            newTok.tok = TOK_BOOL;
            return newTok;
        }

        if(lastChar == '('){
            newTok.tok = TOK_FUNCTIONCALL;
            return newTok;
        }
        

        newTok.tok = TOK_IDENTIFIER;
        return newTok;
    }

    //String lits
    if(lastChar == '"'){
        GetChar();
        newTok.val = "";
        while(lastChar != '"'){
            newTok.val += lastChar;
            GetChar();
        }
        newTok.tok = TOK_STRING;
        lastChar = 0;
        return newTok;
    }

    //compiler instructions
    if(lastChar == '#'){
        GetChar();
        newTok.val = "";
        while (isalnum(lastChar))
        {
            newTok.val += lastChar;
            GetChar();
        }
        newTok.tok = TOK_INSTRUCTION;
        lastChar = 0;

        return newTok;
    }

    switch (lastChar)
    {
    case ',':
        newTok.tok = TOK_COMMA;
        newTok.val = lastChar;
        GetChar();
        return newTok;
        break;

    case '(':
        newTok.tok = TOK_START_PARENTHESIS;
        newTok.val = lastChar;
        GetChar();
        return newTok;
        break;

    case ')':
        newTok.tok = TOK_END_PARENTHESIS;
        newTok.val = lastChar;
        GetChar();
        return newTok;
        break;
    
    default:
        break;
    }

    newTok.val = "";
    while(!isalnum(lastChar) && !isspace(lastChar) && lastChar != EOF && lastChar != ',' && lastChar != '(' && lastChar != ')' && newTok.val != "//"){
        newTok.val += lastChar;
        GetChar();
    }

    if(newTok.val == "//"){
        while(lastChar != '\n' && lastChar != EOF){
            GetChar();
        }
        
        return GetTok();
    }

    if(newTok.val == "!"){
        newTok.tok = TOK_NOT;
        return newTok;
    }

    if(newTok.val == "&&"){
        newTok.tok = TOK_AND;
        return newTok;
    }

    if(newTok.val == "||"){
        newTok.tok = TOK_OR;
        return newTok;
    }

    if(newTok.val == "=="){
        newTok.tok = TOK_EQUALS;
        return newTok;
    }
    
    if(newTok.val == "!="){
        newTok.tok = TOK_NOT_EQUAL;
        return newTok;
    }

    if(newTok.val == ">="){
        newTok.tok = TOK_GREATER_OR_EQUAL;
        return newTok;
    }

    if(newTok.val == "<="){
        newTok.tok = TOK_LESS_OR_EQUAL;
        return newTok;
    }

    if(newTok.val == ">"){
        newTok.tok = TOK_GREATER_THAN;
        return newTok;
    }

    if(newTok.val == "<"){
        newTok.tok = TOK_LESS_THAN;
        return newTok;
    }

    if(newTok.val == "="){
        newTok.tok = TOK_ASSIGN;
        return newTok;
    }

    if(newTok.val == "+="){
        newTok.tok = TOK_PLUS_ASSIGN;
        return newTok;
    }

    if(newTok.val == "-="){
        newTok.tok = TOK_SUB_ASSIGN;
        return newTok;
    }

    if(newTok.val == "*="){
        newTok.tok = TOK_MULTIPLY_ASSIGN;
        return newTok;
    }

    if(newTok.val == "/="){
        newTok.tok = TOK_DIVIDE_ASSIGN;
        return newTok;
    }

    if(newTok.val == "+"){
        newTok.tok = TOK_PLUS;
        return newTok;
    }

    if(newTok.val == "++"){
        newTok.tok = TOK_INCREMENT;
        return newTok;
    }

    if(newTok.val == "-"){
        newTok.tok = TOK_MINUS;
        return newTok;
    }

    if(newTok.val == "--"){
        newTok.tok = TOK_DECREMENT;
        return newTok;
    }

    if(newTok.val == "*"){
        newTok.tok = TOK_MULTIPLY;
        return newTok;
    }

    if(newTok.val == "/"){
        newTok.tok = TOK_DIVIDE;
        return newTok;
    }

    if(newTok.val == "^"){
        newTok.tok = TOK_POWER;
        return newTok;
    }

    if(newTok.val == "."){
        newTok.tok = TOK_DOT;
        return newTok;
    }

    fprintf(stderr, "ERROR: unrecogized symbol: %s\n", newTok.val.c_str());

    return newTok;
}

//Helper function for debugging
//Gets the next token
Token NextToken(){
    Token newTok = GetTok();
    //printf("%d :: %s\n", newTok.tok, newTok.val.c_str());
    return newTok;
}