#include "Node.hpp"
#include <string>
#include <iostream>

std::string VariableExpr::code_gen_let(){
    if(Val.empty()){       
        if(Arg.Type == "auto"){
            fprintf(stderr, "Error: Type inferenced variable %s must be assigned a value at initialization\n", Arg.Name.c_str());
        }
        return Arg.Type + " " + Arg.Name + ";\n";
    }
    return Arg.Type + " " + Arg.Name + " = " + Val + ";\n";
} 

std::string VariableExpr::code_gen_const(){
    if(Val.empty()){
        fprintf(stderr, "Error: Constant %s must be assigned a value at initialization\n", Arg.Name.c_str());
        return "";
    }
    return "const " + Arg.Type + " " + Arg.Name + " = " + Val + ";\n";
}

std::string ArgumentExpr::CodeGen(){
    return Type + " " + Name + ",";
}

std::string BlockExpr::CodeGen(){
    std::string BodyString = "";
    for(size_t i = 0; i < Body.size(); i++){
        BodyString += Body[i];
    }
    return Prototype + "{\n" + BodyString + "}\n";
}

std::string FunctionExpr::CodeGen(){
    if(Args.empty())
        return ReturnType + " " + Name + "()";
        
    return ReturnType + " " + Name + "(" + Args + ")";
}

std::string ReturnExpr::CodeGen(){
    return "return " + Contents + ";\n";
}