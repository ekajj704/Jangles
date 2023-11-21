#include "../headers/lexer.hpp"
#include "../headers/function.hpp"
#include "../headers/debugprint.hpp"
#include "../headers/parser.hpp"
#include "../headers/exec.hpp"
#include "../libs/librarian.hpp"
#include <algorithm>

/*
    ------------------------------------------------------------------------------------------
    This file is used to run the tokens
    ------------------------------------------------------------------------------------------
*/

//Template Token Vectors
#define TRUE vector<Token>({Token({TOK_BOOL, "true"})});
#define FALSE vector<Token>({Token({TOK_BOOL, "false"})});
#define NULL_TOKEN vector<Token>({Token({0, ""})});

//A map of all variables in the program
auto variables = vector<map<string, Token>>();
Function* currentFunction;
vector<Token> functionReturn = NULL_TOKEN;
vector<int> currentLine;
bool hasReturned = false;
int currentTabLevel = 0;

/*
    ------------------------------------------------------------------------------------------
    Helper functions & Variable retrieval
    ------------------------------------------------------------------------------------------
*/

//Checks if the token is a literal
bool IsLiteral(Token token){
    switch (token.tok)
    {
    case TOK_NUMBER:
        return true;
        break;
    case TOK_STRING:
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

double TokenToDouble(vector<Token>* num){
    return strtod(num->front().val.c_str(), nullptr);
}

vector<Token> TokenToBool(vector<Token>* vec){
    if(vec->front().tok == TOK_BOOL){
        if(vec->front().val == "true"){
            return TRUE;
        }
        return FALSE;
    }

    if(vec->front().tok == TOK_NUMBER && TokenToDouble(vec) != 0){
        return TRUE;
    }

    if(vec->front().val != ""){
        return TRUE;
    }

    return FALSE;
}

string DoubleToString(double num){
    double i;
    if(modf(num, &i) == 0){
        long int newNum = i;
        return to_string(newNum);
    }

    return to_string(num);
}

void BurnTabLevels(){
    for(int i = variables.size() - 1; i > currentTabLevel; i--){
        variables.pop_back();
    }
}

int GetTabLevel(vector<Token>* expr){
    int tabLevel = 0;
    while(expr->front().tok == TOK_TAB && expr->size() > 0){
        expr->erase(expr->begin());
        tabLevel++;
    }

    return tabLevel;
}

//Gets the value of a variable if it exists
void ExecGetVar(vector<Token>* var){

    for(int i = 0; i < currentTabLevel + 1; i++){
        if(variables.at(i).count(var->front().val) == 1){
            var->front().tok = variables.at(i).at(var->front().val).tok;
            var->front().val = variables.at(i).at(var->front().val).val;
            return;
        }
    }
    PrintError("Variable: \"" + var->front().val + "\" was never declared");
    exit(1);
}

//If the token is a variable, return a variable
void CheckVar(vector<Token>* tok){
    if(tok->front().tok == TOK_IDENTIFIER)
        ExecGetVar(tok);
        return;

    return;
}

//Checks if the token is a boolean and returns converts it to a 1 is it is
void CheckBool(vector<Token>* tok){
    if(tok->front().tok != TOK_BOOL)
        return;
    
    tok->front().tok = TOK_NUMBER;

    if(tok->front().val == "true"){
        tok->front().val = "1";
        return;
    }

    tok->front().val = "0";
}

/*
    ------------------------------------------------------------------------------------------
    Basic Math Operations
    ------------------------------------------------------------------------------------------
*/

//Handling division
vector<Token> ExecDivision(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok == 0 || RHS->front().tok == 0){
        PrintError("Cannot use operator '/' on type of <NULL>");
    }

    //Blocks the division of strings
    if(LHS->front().tok == TOK_STRING || RHS->front().tok == TOK_STRING){
        PrintError("Cannot use operator '/' on type of <String>");
        exit(1);
    }

    //Returns and divides the remaining numbers
    auto newToken = NULL_TOKEN;

    newToken.front().tok = TOK_NUMBER;
    newToken.front().val = DoubleToString(TokenToDouble(LHS) / TokenToDouble(RHS));

    return newToken;
}

//Handling multiplications
vector<Token> ExecMultiply(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok == 0 || RHS->front().tok == 0){
        PrintError("Cannot use operator '*' on type of <NULL>");
    }

    auto newToken = NULL_TOKEN;

    //Blocks the multiplication of strings
    if(LHS->front().tok == TOK_STRING && RHS->front().tok == TOK_STRING){
        PrintError("Cannot use operator '*' on types <String> and <String>");
        exit(1);
    }

    //Multiplying a string by a number
    //If the number is positive it returns a string with multiple copies of the string
    //If the number is reversed it returns a string with multiple copies of the string reversed
    if(LHS->front().tok == TOK_STRING){
        if(TokenToDouble(RHS) < 0){
            reverse(LHS->front().val.begin(), LHS->front().val.end());
            RHS->front().val = DoubleToString(TokenToDouble(RHS) * -1);
        }
        newToken.front().tok = TOK_STRING;
        newToken.front().val = "";
        for(int i = 0; i < TokenToDouble(RHS); i++){
            newToken.front().val += LHS->front().val;
        }
        return newToken;
    }

    //Same as the if above
    if(RHS->front().tok == TOK_STRING){
        if(TokenToDouble(LHS) < 0){
            reverse(RHS->front().val.begin(), RHS->front().val.end());
            LHS->front().val = DoubleToString(TokenToDouble(LHS) * -1);
        }
        newToken.front().tok = TOK_STRING;
        newToken.front().val = "";
        for(int i = 0; i < TokenToDouble(LHS); i++){
            newToken.front().val += RHS->front().val;
        }
        return newToken;
    }

    //Multiplies numbers together and returns it
    newToken.front().tok = TOK_NUMBER;
    newToken.front().val = DoubleToString(TokenToDouble(LHS) * TokenToDouble(RHS));

    return newToken;
}

//Handling powers
vector<Token> ExecPower(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok == 0 || RHS->front().tok == 0){
        PrintError("Cannot use operator '^' on type of <NULL>");
    }

    //Stops the use of strings in powers
    if(LHS->front().tok == TOK_STRING || RHS->front().tok == TOK_STRING){
        PrintError("Cannot use operator '^' on type of <String>");
        exit(1);
    }

    //returns the number to a power
    auto newToken = NULL_TOKEN;

    newToken.front().tok = TOK_NUMBER;
    newToken.front().val = DoubleToString(pow(TokenToDouble(LHS), TokenToDouble(RHS)));

    return newToken;
}

//Adding two literals
vector<Token> ExecAdd(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok == 0 || RHS->front().tok == 0){
        PrintError("Cannot use operator '+' on type of <NULL>");
    }

    auto newToken = NULL_TOKEN;
    
    //Appends anything to a string
    if(LHS->front().tok == TOK_STRING || RHS->front().tok == TOK_STRING){   
        newToken.front().tok = TOK_STRING;
        newToken.front().val = LHS->front().val + RHS->front().val;
        return newToken;
    }

    //Returns the sum of two numbers
    newToken.front().tok = TOK_NUMBER;
    newToken.front().val = DoubleToString(TokenToDouble(LHS) + TokenToDouble(RHS));
    return newToken;

}


//Subtracting two literals
vector<Token> ExecSub(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    auto newToken = NULL_TOKEN;

    if(LHS->front().tok == 0 || RHS->front().tok == 0){
        PrintError("Cannot use operator '-' on type of <NULL>");
    }

    //Blocks the subtraction of strings from strings
    if(LHS->front().tok == TOK_STRING && RHS->front().tok == TOK_STRING){
        PrintError("Cannot use operator '-' on types <String> and <String>");
        exit(1);
    }

    //if there is nothing in LHS, make RHS a negative number
    if(LHS->front().tok == 0 && RHS->front().tok == TOK_NUMBER){
        double num = TokenToDouble(RHS);
        newToken.front().tok = TOK_NUMBER;
        newToken.front().val = DoubleToString(num * -1);
        return newToken;
    }

    //Removes RHS number of chars from the end of the LHS string
    if(LHS->front().tok == TOK_STRING){
        size_t num = TokenToDouble(RHS);
        newToken.front().tok = TOK_STRING;
        newToken.front().val = LHS->front().val.substr(0, LHS->front().val.length() - (num <= RHS->front().val.size()? num : RHS->front().val.size()));
        return newToken;
    }

    ////Removes LHS number of chars from the beginning of the RHS string
    if(RHS->front().tok == TOK_STRING){
        size_t num = TokenToDouble(LHS);
        newToken.front().tok = TOK_STRING;
        newToken.front().val = RHS->front().val.substr(num <= RHS->front().val.size()? num : RHS->front().val.size());
        return newToken;
    }
    
    //Returns LHS - RHS if both are numbers
    newToken.front().tok = TOK_NUMBER;
    newToken.front().val = DoubleToString(TokenToDouble(LHS) - TokenToDouble(RHS));

    return newToken;
}

/*
    ------------------------------------------------------------------------------------------
    Boolean Operations
    ------------------------------------------------------------------------------------------
*/

vector<Token> ExecAnd(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER || RHS->front().tok != TOK_NUMBER){
        PrintError("Boolean statements require integral or enumerated types");
        exit(-1);
    }

    if(TokenToDouble(LHS) != 0 && TokenToDouble(RHS) != 0){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecOr(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER || RHS->front().tok != TOK_NUMBER){
        PrintError("Boolean statements require integral or enumerated types");
        exit(-1);
    }

    if(TokenToDouble(LHS) != 0 && TokenToDouble(RHS) != 0){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecEqual(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok == RHS->front().tok && LHS->front().val == RHS->front().val){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecNot(vector<Token>* RHS){
    CheckVar(RHS);

    if(TokenToBool(RHS).front().val == "true"){
        return FALSE;
    }

    return TRUE;
}

vector<Token> ExecNotEqual(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != RHS->front().tok || LHS->front().val != RHS->front().val){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecGreaterThan(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER && RHS->front().tok != TOK_NUMBER){
        PrintError("Cannot use operator '>' on non-numerical types");
    }

    if(TokenToDouble(LHS) > TokenToDouble(RHS)){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecLessThan(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER && RHS->front().tok != TOK_NUMBER){
        PrintError("Cannot use operator '<' on non-numerical types");
    }

    if(TokenToDouble(LHS) < TokenToDouble(RHS)){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecGreaterThanOrEqual(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER && RHS->front().tok != TOK_NUMBER){
        PrintError("Cannot use operator '>=' on non-numerical types");
    }

    if(LHS->front().tok == RHS->front().tok && LHS->front().val == RHS->front().val){
        return TRUE;
    }

    if(TokenToDouble(LHS) < TokenToDouble(RHS)){
        return TRUE;
    }

    return FALSE;
}

vector<Token> ExecLessThanOrEqual(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(LHS);
    CheckVar(RHS);
    CheckBool(LHS);
    CheckBool(RHS);

    if(LHS->front().tok != TOK_NUMBER && RHS->front().tok != TOK_NUMBER){
        PrintError("Cannot use operator '<=' on non-numerical types");
        exit(-1);
    }

    if(LHS->front().tok == RHS->front().tok && LHS->front().val == RHS->front().val){
        return TRUE;
    }

    if(TokenToDouble(LHS) < TokenToDouble(RHS)){
        return TRUE;
    }

    return FALSE;
}

/*
    ------------------------------------------------------------------------------------------
    Variable assignment and declaration
    ------------------------------------------------------------------------------------------
*/

//Handling variable declaration
void ExecLet(Token* name){
    if(name->tok != TOK_IDENTIFIER){
        if(name->tok < 0){
            PrintError("Expected Identifier, instead got: " + TOKEN_TYPE_NAMES.at(name->tok));
            exit(1);
        }

        PrintError("Expected Identifier, instead got: " + name->tok);
    }

    variables.at(currentTabLevel).insert({name->val, Token({0, ""})});
}

//Handleing variable assignment
vector<Token> ExecAssign(vector<Token>* LHS, vector<Token>* RHS){
    CheckVar(RHS);
    bool isDeclared = false;

    if(LHS->front().tok != TOK_IDENTIFIER){
        PrintError("Cannot assign to " + TOKEN_TYPE_NAMES.at(LHS->front().tok));
        exit(-1);
    }
    
    for(int i = 0; i < currentTabLevel + 1; i++){
        if(variables.at(i).count(LHS->front().val) == 1){
            auto newToken = NULL_TOKEN;
            newToken.front().tok = RHS->front().tok;
            newToken.front().val = RHS->front().val;
            variables.at(i).at(LHS->front().val) = newToken.front();
            isDeclared = true;
            break;
        }
    }

    if(!isDeclared){
        PrintError("Variable: \"" + LHS->front().val + "\" was never declared");
        exit(1);
    }
    

    return *LHS;
}

//Assings the sum the var in LHS and RHS then assigns it to the var
vector<Token> ExecPlusAssign(vector<Token>* LHS, vector<Token>* RHS){
    auto var = *LHS;
    auto buff = ExecAdd(LHS, RHS);
    return ExecAssign(&var, &buff);
}

//Assings the difference the var in LHS and RHS then assigns it to the var
vector<Token> ExecMinusAssign(vector<Token>* LHS, vector<Token>* RHS){
    auto var = *LHS;
    auto buff = ExecSub(LHS, RHS);
    return ExecAssign(&var, &buff);
}

//Assings the product the var in LHS and RHS then assigns it to the var
vector<Token> ExecMultiplyAssign(vector<Token>* LHS, vector<Token>* RHS){
    auto var = *LHS;
    auto buff = ExecMultiply(LHS, RHS);
    return ExecAssign(&var, &buff);
}

//Assings the quotient the var in LHS and RHS then assigns it to the var
vector<Token> ExecDivideAssign(vector<Token>* LHS, vector<Token>* RHS){
    auto var = *LHS;
    auto buff = ExecDivision(LHS, RHS);
    return ExecAssign(&var, &buff);
}


//Returns LHS + 1
vector<Token> ExecIncrement(vector<Token>* LHS){
    auto one = vector<Token>({Token({TOK_NUMBER, "1"})});
    if(LHS->front().tok == TOK_IDENTIFIER){
        return ExecPlusAssign(LHS, &one);
    }

    return ExecAdd(LHS, &one);
}

//Returns LHS - 1
vector<Token> ExecDecrement(vector<Token>* LHS){
    auto one = vector<Token>({Token({TOK_NUMBER, "1"})});
    if(LHS->front().tok == TOK_IDENTIFIER){
        return ExecMinusAssign(LHS, &one);
    }

    return ExecSub(LHS, &one);
}

//Returns a list of elements when they are seperated by a comma
vector<Token> ExecComma(vector<Token>* LHS, vector<Token>* RHS){
    vector<Token> newVec;
    newVec.insert(newVec.end(), LHS->begin(), LHS->end());
    newVec.insert(newVec.end(), RHS->begin(), RHS->end());

    return newVec;
}

/*
    ------------------------------------------------------------------------------------------
    Conditional Execution
    ------------------------------------------------------------------------------------------
*/

vector<Token> ExecIf(vector<Token>* LHS, vector<Token>* RHS){
    string expressionVal = RHS->front().val;
    auto ifFunc = Function();
    ifFunc.SetName("if");

    currentLine.back()++;
    vector<Token> buff;
    while(currentLine.back() < currentFunction->LineCount() && GetTabLevel(&(buff = currentFunction->GetLine(currentLine.back()))) >= currentTabLevel + 1){
        ifFunc.AddLine(&(buff = currentFunction->GetLine(currentLine.back())));
        currentLine.back()++;
    }

    vector<Token> buff2;
    if(TokenToBool(&(buff2 = ExecOp(&(buff = *RHS)))).front().val == "true" && hasReturned == false){
        int ifReturned = ExecFunction(&ifFunc);
        hasReturned = ifReturned ? true : false;
    }

    return TRUE;

}

vector<Token> ExecWhile(vector<Token>* LHS, vector<Token>* RHS){
    string expressionVal = RHS->front().val;
    auto whileFunc = Function();
    whileFunc.SetName("while");

    currentLine.back()++;
    vector<Token> buff;
    while(currentLine.back() < currentFunction->LineCount() && GetTabLevel(&(buff = currentFunction->GetLine(currentLine.back()))) >= currentTabLevel + 1){
        whileFunc.AddLine(&(buff = currentFunction->GetLine(currentLine.back())));
        currentLine.back()++;
    }

    vector<Token> buff2;
    while(TokenToBool(&(buff2 = ExecOp(&(buff = *RHS)))).front().val == "true" && hasReturned == false){
        int ifReturned = ExecFunction(&whileFunc);
        hasReturned = ifReturned ? true : false;
    }

    return TRUE;

}


/*
    ------------------------------------------------------------------------------------------
    Line Execution
    ------------------------------------------------------------------------------------------
*/

//Executing lines that don't have an operator
vector<Token> ExecExpression(vector<Token>* expr){

    //Checks if the statement is a function call and executes it
    if(expr->front().tok == TOK_FUNCTIONCALL){
        vector<Token> args;
        if(expr->size() > 1){
            auto args = vector<Token>(expr->begin() + 1, expr->end());
            CallFunction(expr->front().val, &args);
        }
        CallFunction(expr->front().val);
        return functionReturn;
    }

    if(expr->size() == 0){
        return NULL_TOKEN;
    }

    //Returns itself if it is the length of 1
    if(expr->size() == 1){
        return *expr;
    }

    //Checks if it is a variable declaration and executes it
    if(expr->front().tok == TOK_LET){
        expr->erase(expr->begin());
        ExecLet(&(expr->front()));
        return *expr;
    }

    //returns a empty token if it doesn't match everthing before
    return NULL_TOKEN;
}

//Execute lines with Operators in them
vector<Token> ExecOp(vector<Token>* expr){
    int opIndex = -1;
    int op = 99999;
    int parenthesisCount = 0;


    if(expr->size() == 0){
        return NULL_TOKEN;
    }

    for(size_t i = 0; i < expr->size(); i++){
        if(expr->at(i).tok > 0 && expr->at(i).tok <= op){
                opIndex = i;
                op = expr->at(i).tok;
        }
    }
    

    if(op == TOK_START_PARENTHESIS){
        int endParenIndex;
        for(size_t i = 0; i < expr->size(); i++){
            if(expr->at(i).tok == TOK_END_PARENTHESIS){
                endParenIndex = i;
            }
        }

        if(endParenIndex - opIndex <= 1){
            auto LHS = vector<Token>(expr->begin(), expr->begin() + opIndex);
            auto RHS = vector<Token>(vector<Token>(expr->begin() + opIndex + 2, expr->end()));
            LHS = ExecOp(&LHS);

            LHS.insert(LHS.end(), RHS.begin(), RHS.end());

            return ExecOp(&LHS);
        }

        if(!endParenIndex)
            PrintError("Expected ')'");
                
        auto LHS = vector<Token>(expr->begin(), expr->begin() + opIndex);
        auto Middle = vector<Token>(expr->begin() + opIndex + 1, expr->begin() + endParenIndex);
        auto RHS = vector<Token>(vector<Token>(expr->begin() + endParenIndex + 1, expr->end()));
        auto MiddleLit = ExecOp(&Middle);

        LHS.insert(LHS.end(), MiddleLit.begin(), MiddleLit.end());
        LHS.insert(LHS.end(), RHS.begin(), RHS.end());
        
        return ExecOp(&LHS);

    }

    if(op == TOK_MINUS && (opIndex == 0 || expr->at(opIndex - 1).tok > 0) && expr->at(opIndex + 1).tok == TOK_NUMBER){
        auto blank = NULL_TOKEN;
        auto number = vector<Token>({expr->at(opIndex + 1)});
        expr->at(opIndex) = ExecSub(&blank, &number).front();
        expr->erase(expr->begin() + opIndex + 1);
        return ExecOp(expr);
    }

    if(op == TOK_WHILE){
        auto blank = NULL_TOKEN;
        auto boolExpr = vector<Token>(expr->begin() + 1, expr->end());
        return ExecWhile(&blank, &boolExpr);
    }

    if(op == TOK_NOT){
        auto boolExpr = vector<Token>(expr->begin() + 1, expr->end());
        return ExecNot(&boolExpr);
    }
    
    if(opIndex >= 0){
        auto LHS = vector<Token>(expr->begin(), expr->begin() + opIndex);
        auto RHS = NULL_TOKEN;

        if(opIndex != expr->size() - 1){
            RHS = vector<Token>(expr->begin() + opIndex + 1, expr->end());
        }

        vector<Token> leftLit;
        
        if(op != TOK_RETURN){
            leftLit = ExecOp(&LHS);
        }
        auto rightLit = ExecOp(&RHS);

        switch (op)
        {
        case TOK_PLUS:
            return ExecAdd(&leftLit, &rightLit);
            break;
        
        case TOK_INCREMENT:
            return ExecIncrement(&leftLit);
            break;
        
        case TOK_DECREMENT:
            return ExecDecrement(&leftLit);
            break;
        
        case TOK_MINUS:
            return ExecSub(&leftLit, &rightLit);
            break;
        
        case TOK_MULTIPLY:
            return ExecMultiply(&leftLit, &rightLit);
            break;
        
        case TOK_DIVIDE:
            return ExecDivision(&leftLit, &rightLit);
            break;
        
        case TOK_POWER:
            return ExecPower(&leftLit, &rightLit);
            break;
        
        case TOK_ASSIGN:
            return ExecAssign(&leftLit, &rightLit);
            break;
        
        case TOK_PLUS_ASSIGN:
            return ExecPlusAssign(&leftLit, &rightLit);
            break;
        
        case TOK_SUB_ASSIGN:
            return ExecMinusAssign(&leftLit, &rightLit);
            break;
        
        case TOK_MULTIPLY_ASSIGN:
            return ExecMultiplyAssign(&leftLit, &rightLit);
            break;
        
        case TOK_DIVIDE_ASSIGN:
            return ExecDivideAssign(&leftLit, &rightLit);
            break;
        
        case TOK_COMMA:
            return ExecComma(&leftLit, &rightLit);
            break;
        
        case TOK_AND:
            return ExecAnd(&leftLit, &rightLit);
            break;
        
        case TOK_OR:
            return ExecOr(&leftLit, &rightLit);
            break;
        
        case TOK_EQUALS:
            return ExecEqual(&leftLit, &rightLit);
            break;
        
        case TOK_NOT_EQUAL:
            return ExecNotEqual(&leftLit, &rightLit);
            break;
        
        case TOK_GREATER_THAN:
            return ExecGreaterThan(&leftLit, &rightLit);
            break;
        
        case TOK_LESS_THAN:
            return ExecLessThan(&leftLit, &rightLit);
            break;
        
        case TOK_GREATER_OR_EQUAL:
            return ExecGreaterThanOrEqual(&leftLit, &rightLit);
            break;
        
        case TOK_LESS_OR_EQUAL:
            return ExecLessThanOrEqual(&leftLit, &rightLit);
            break;
        
        case TOK_IF:
            return ExecIf(&leftLit, &rightLit);
            break;
        
        case TOK_RETURN:
            functionReturn = rightLit;
            PrintVectorDebug(&rightLit);
            hasReturned = true;
            return NULL_TOKEN;
            break;
        
        case TOK_DOT:
            return RunBookFunc(&leftLit, &rightLit);
            break;


        default:
            break;
        }

    }
    return ExecExpression(expr);
}

vector<Token> ExecLine(vector<Token>* expr){
    currentTabLevel = GetTabLevel(expr);
    BurnTabLevels();
    return ExecOp(expr);
}

/*
    ------------------------------------------------------------------------------------------
    Function Execution
    ------------------------------------------------------------------------------------------
*/

int ExecFunction(Function* func){
    auto parentFunction = currentFunction;
    currentFunction = func;
    currentLine.push_back(0);
    variables.push_back(map<string, Token>());

    while(currentLine.back() < func->LineCount()){
        if(hasReturned){
            hasReturned = false;
            break;
        }
        auto buff = func->GetLine(currentLine.back());
        buff = ExecLine(&buff);
        PrintVectorDebug(&buff);
        currentLine.back()++;
    }

    currentLine.pop_back();

    currentFunction = parentFunction;

    if(hasReturned){
        return 1;
    }

    return 0;
}

int ExecFunction(Function* func, vector<Token>* args){
    size_t i = 0;
    auto parentFunction = currentFunction;
    currentFunction = func;
    variables.push_back(map<string, Token>());
    currentLine.push_back(0);

    while(i < func->ArgCount()){
        auto buff = Token({TOK_IDENTIFIER, func->GetArg(i)});
        ExecLet(&buff);
        i++;
    }

    i = 0;
    while(i < args->size()){
        if(i < func->ArgCount()){
            auto left = vector<Token>({Token({TOK_IDENTIFIER, func->GetArg(i)})});
            auto right = vector<Token>({Token({args->at(i).tok, args->at(i).val})});
            ExecAssign(&left, &right);
        }
        i++;
    }
    while(currentLine.back() < func->LineCount()){
        if(hasReturned){
            hasReturned = false;
            break;
        }
        auto buff = func->GetLine(currentLine.back());
        buff = ExecLine(&buff);
        currentLine.back()++;
    }

    currentLine.pop_back();
    currentFunction = parentFunction;
    if(hasReturned){
        return 1;
    }

    return 0;
}