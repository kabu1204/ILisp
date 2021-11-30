//
// Created by yuchengye on 2021/11/30.
//

#ifndef ILISP_PARSE_HPP
#define ILISP_PARSE_HPP
#include "util.hpp"
#include "cmath"
#include "re2/re2.h"
using namespace re2;

struct symbol{
    bool isSymbol;
    std::string name;
    value_t value_type;
    union{ int num; double fp; } value;
    template<class T>
    void setValue(T _value){
        if(std::is_same<int, T>::value){ value.num=_value; value_type=Int;}
        else{ value.fp=_value; value_type=Double; }
    }
    template<class T>
    symbol(T _value):isSymbol(false){ setValue(_value); }
    template<class T>
    symbol(std::string _name, T _value):isSymbol(true),name(_name){ setValue(_value); }
};

struct Value{
    value_t type;
    union{int Integer; double Fp;} data;
    template<class T>
    T get(){
        if(type==Int) return data.Integer;
        return data.Fp;
    }
    Value(double _data):type(Double){data.Fp=_data;}
    Value(int _data):type(Int){data.Integer=_data;}
    Value():type(Int){data.Integer=0;}
};

std::unordered_map<std::string, Value> Env_symbol{std::make_pair("PI", Value(3.14159265358979))};
std::unordered_map<std::string, std::function<Value(std::vector<Value>)>> Env_Op{
    std::make_pair("_ret",[](std::vector<Value> args)->Value{return args[0];}),
    std::make_pair("if", [](std::vector<Value> args)->Value{ return args[0].get<int>()?args[1]:args[2]; }),
    std::make_pair("+", [](std::vector<Value> args)->Value{
        double res=0;
        for(auto v:args) res+=v.get<double>();
        return {res};
    }),
    std::make_pair("-", [](std::vector<Value> args)->Value{ return {args[0].get<double>()-args[1].get<double>()}; }),
    std::make_pair("*",[](std::vector<Value> args)->Value{
        double res=1;
        for(auto v:args) res*=v.get<double>();
        return {res};
    }),
    std::make_pair("/",[](std::vector<Value> args)->Value{return {args[0].get<double>()/args[1].get<double>()}; }),
    std::make_pair(">",[](std::vector<Value> args)->Value{return {args[0].get<double>()>args[1].get<double>()}; }),
    std::make_pair("=",[](std::vector<Value> args)->Value{return {args[0].get<double>()==args[1].get<double>()}; }),
    std::make_pair("<",[](std::vector<Value> args)->Value{return {args[0].get<double>()<args[1].get<double>()}; }),
    std::make_pair("<>",[](std::vector<Value> args)->Value{return {args[0].get<double>()!=args[1].get<double>()}; }),
    std::make_pair("begin",[](std::vector<Value> args)->Value{dd("begin!:%d\n",args.back().get<int>());return args.back(); }),
    std::make_pair("max",[](std::vector<Value> args)->Value{
        double res=args[0].get<double>();
        for(auto v:args) res = (res>v.get<double>())?res:v.get<double>();
        return {res};
    }),
    std::make_pair("min",[](std::vector<Value> args)->Value{
        double res=args[0].get<double>();
        for(auto v:args) res = (res<v.get<double>())?res:v.get<double>();
        return {res};
    }),
    std::make_pair("abs",[](std::vector<Value> args)->Value{ return {abs(args[0].get<double>())}; }),
    std::make_pair("sqrt",[](std::vector<Value> args)->Value{ return {sqrt(args[0].get<double>())}; }),
    std::make_pair("sin",[](std::vector<Value> args)->Value{ return {sin(args[0].get<double>())}; }),
    std::make_pair("cos",[](std::vector<Value> args)->Value{ return {cos(args[0].get<double>())}; }),
    std::make_pair("tan",[](std::vector<Value> args)->Value{ return {tan(args[0].get<double>())}; }),
    std::make_pair("log10",[](std::vector<Value> args)->Value{ return {log10(args[0].get<double>())}; }),
    std::make_pair("log2",[](std::vector<Value> args)->Value{ return {log2(args[0].get<double>())}; }),
    std::make_pair("ln",[](std::vector<Value> args)->Value{ return {log(args[0].get<double>())}; }),
};

RE2 Number(R"(^-?\+?0|[1-9]\d*$)");
RE2 Float(R"(^-?\d+\.\d+$)");

struct AST{
    AST* parent;
    std::string Op;
    std::vector<Value> args;
    AST(AST* _parent, std::string _Op):parent(_parent),Op(_Op){}
};

bool isNumber(const std::string& content, StringPiece*& group){
    if(Number.Match(content,0,(content).size(),RE2::ANCHOR_BOTH,group,1)){
        return true;
    }
    return false;
}

bool isFloat(const std::string& content, StringPiece*& group){
    if(Float.Match(content,0,(content).size(),RE2::ANCHOR_BOTH,group,1)){
        return true;
    }
    return false;
}

bool isSymbol(const std::string& content, StringPiece*& group){
    if(Env_symbol.find((content)) != Env_symbol.end()){
        return true;
    }
    return false;
}

Value List(Stack<std::string>& stk, AST* parent){
    std::string content;
    StringPiece *group=new StringPiece[2];
    while(!stk.empty()){
        stk.pop(content);
        dd("content:%s\n",content.c_str());
        if(isNumber(content,group)){
            parent->args.emplace_back(std::stoi(content));
            dd("Number:%d\n",std::stoi(content));
        }
        else if(isFloat(content,group)){
            parent->args.emplace_back(std::stod(content));
            dd("Float:%.6f\n",std::stod(content));
        }
        else if(isSymbol(content,group)){
            parent->args.push_back(Env_symbol[content]);
            dd("Symbol:%s\n",content.c_str());
        }
        else if(content==")"){
            break;
        }
        else if(content=="("){
            // is a List
            std::string op_code = stk.pop();
            if(op_code=="define"){
                std::string symbol_name=stk.pop();
                AST* expr=new AST(nullptr,"_ret");
                Env_symbol[symbol_name]=List(stk, expr);
                dd("defined:%s=%d\n",symbol_name.c_str(),Env_symbol[symbol_name].get<int>());
            }
            else{
                AST* Op=new AST(nullptr,op_code);
                parent->args.push_back(List(stk, Op));
            }
        }
        else{
            dd("keyword:%s\n",content.c_str());
        }
    }
    dd("Op:%s\n",parent->Op.c_str());
    if(Env_Op.find(parent->Op)!=Env_Op.end()){
        return Env_Op[parent->Op](parent->args);
    } else {
        dd("Undefined symbol:%s\n",parent->Op.c_str());
        return {0};
    }
}

#endif //ILISP_PARSE_HPP
