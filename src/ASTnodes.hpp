#ifndef _ASTNODES_H
#define _ASTNODE_H
#include <string>
#include <memory>
#include <vector>

class ExprAST{

    public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST{
    double Val;

    public:
    NumberExprAST(double Val): Val(Val) {}
};

class VariableExprAST: public ExprAST{
    std::string Name;

    public:
    VariableExprAST(const std::string Name): Name(Name) {}
};

class BinaryExprAST: public ExprAST{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): 
        Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST: public ExprAST{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

    public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args): 
        Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST{
    std::string Name;
    std::vector<std::unique_ptr<ExprAST>> Args;

    PrototypeAST(std::string Name, std::vector<std::unique_ptr<ExprAST>> Args): Name(Name), Args(std::move(Args)) {}

};

class FunctionAST{
    std::unique_ptr<PrototypeAST> Prototype;
    std::unique_ptr<ExprAST> Body;

    public:
    FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExprAST> Body): 
        Prototype(std::move(Prototype)), Body(std::move(Body)) {}
};

#endif