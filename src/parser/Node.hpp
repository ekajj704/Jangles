#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <memory>
#include <vector>

class Expr{

    public:
        virtual ~Expr() =  default;
};

class ImportExpr: Expr{
    std::string Name;

    public:
        ImportExpr(std::string Name): Name(Name){}
};

class ArgumentExpr: Expr{
    
    public:
        std::string Name;
        std::string Type;
        
        ArgumentExpr(std::string Name, std::string Type): Name(Name), Type(Type) {}
        std::string CodeGen();
};

class VariableExpr: Expr{
    

    public:
        ArgumentExpr Arg;
        std::string Val;

        VariableExpr(ArgumentExpr Arg, std::string Val): Arg(Arg), Val(Val) {}
        std::string code_gen_let();
        std::string code_gen_const();
};

class ConstExpr: Expr{
    ArgumentExpr Arg;
    std::string Val;

    public:
        ConstExpr(ArgumentExpr Arg, std::string Val): Arg(Arg), Val(Val) {} 
};

class ReturnExpr: Expr{
    std::string Contents;

    public:
        ReturnExpr(std::string Contents): Contents(Contents) {}
        std::string CodeGen();
};

class BlockExpr: Expr{
    

    public:
        std::string Prototype;
        std::vector<std::string> Body;

        void AddLine(std::string line){
            Body.push_back(line);
        }

        BlockExpr() = default;
        virtual std::string CodeGen();
};

class FunctionExpr: BlockExpr{
    

    public:
        std::string Name;
        std::string ReturnType;
        std::string Args;
        FunctionExpr(std::string Name, std::string ReturnType, std::string Args): Name(Name), ReturnType(ReturnType), Args(Args) {}
        FunctionExpr(){}
        std::string CodeGen();
};

class TableExpr: BlockExpr{
    std::string Name;

    public:
        TableExpr(std::string Name): Name(Name){}
};

class ConditionalExpr: BlockExpr{
    std::string Name;
    std::string Condition;

    public:
        ConditionalExpr(std::string Name, std::string Condition): Name(Name), Condition(Condition){}
};


#endif