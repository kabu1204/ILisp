//
// Created by yuchengye on 2021/11/29.
//

#ifndef ILISP_UTIL_HPP
#define ILISP_UTIL_HPP
#include "vector"
#include "string"
#include "unordered_set"
#include "unordered_map"
#define MAX_STACK_SIZE 1024  // 1KB
using value_t=uint8_t;
#define Int 0
#define Double 1
#define Str 2
#define Symbol 3

#define FILENAME_ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __METHOD__ (strchr(__PRETTY_FUNCTION__, ' ')+1)

#ifdef DEBUG
#define dd(...) do{std::printf("\033[1m\033[31m%s\033[0m \033[36min\033[0m \033[1m\033[4m\033[32m%s:\033[1m\033[4m\033[35m%d\033[0m\n\t",FILENAME_,__METHOD__,__LINE__); \
std::printf(__VA_ARGS__);}while(0)
#else
#define dd(...); ;
#endif

/**
 * @brief a simple of data structure stack
 */
template<class T>
struct Stack {
    T* stack_space; T* sp; uint32_t capacity;
    Stack(size_t stack_size=MAX_STACK_SIZE): capacity(stack_size / sizeof(T)), stack_space((T*)malloc(capacity*sizeof(T))), sp(stack_space){
        dd("stack_space:%p\n",stack_space);
        dd("addr of stack_space[0]:%p\n",&stack_space[0]);
        dd("sp:%p\n",sp);
    }
    inline void push(T&& e) { if(where() < capacity - 1) *(sp++)=e; else dd("StackOverflow\n");}
    inline T pop() { if(where()>0) return *(--sp); else dd("EmptyStack\n"); }
    inline void pop(T& e) { if(where()>0) e=*(--sp); else dd("EmptyStack\n"); }
    inline T top() { if(where()>0) return *(sp-sizeof(T)); else dd("EmptyStack\n");}
    inline int where() const { return sp-stack_space; }
    inline void clear(){ sp=stack_space; }
    inline void clean(){ clear(); free(stack_space); }
    inline bool empty() const { return where()==0; }
    inline uint32_t size() const { return where(); }
    inline void reverse() {
        T* t_space=(T*)malloc(capacity*sizeof(T)); T* tp=t_space;
        while(!empty()) *(tp++)=*(--sp);
        free(stack_space); stack_space=t_space; sp=tp;
    }
    Stack(const Stack &x) { capacity = x.capacity; stack_space=(T*)malloc(capacity * sizeof(T)); sp=stack_space; T* sp_x=x.stack_space;
        for(int i=0;i<x.where();++i) *(sp++)=*(sp_x++); dd("copied\n");}
    ~Stack(){clean();}
};

Stack<std::string> split(const std::string& str, const std::string& delim) {
    Stack<std::string> ret;
    if(str.empty()) return ret;
    char *_str = new char[str.length() + 1];
    strcpy(_str, str.c_str());
    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());
    char *p = strtok(_str, d);
    while(p){
        std::string s = p;
        ret.push(std::move(s));
        p = strtok(nullptr, d);
    }
    dd("size of ret:%d\n", ret.size());
    return ret;
}

Stack<std::string> tokenize(std::string str){
//    dd("length of str:%d\n", str.length());
    for(int i=str.length()-1;i>=0;i--){
//        std::printf("char of pos%d:%c\n",i,str[i]);
        if(str[i]=='(') {str.insert(i+1,1,' '); continue;}
        if(str[i]==')') {str.insert(i,1,' '); continue;}
    }

    dd("%s\n", str.c_str());

    Stack<std::string> tokens_split_by_spc = split(str, " ");

    Stack<std::string> tokens(tokens_split_by_spc);
    dd("size of token:%d\n",tokens.size());
    return tokens;
}

int parse(Stack<std::string> str_v){
    std::vector<std::string> parse_stack;
    std::string str;
    while(!str_v.empty()){
        str_v.pop(str);
        if(str==")"){
            std::vector<std::string> list;
            str_v.pop(str);
            while(str!="(" && !str_v.empty()){
                list.push_back(str);
                str_v.pop(str);
            }
            std::string symbol=list.back();
            dd("Symbol:%s\n", symbol.c_str());
            if(symbol=="+"){
                assert(list.size()==3);
                return std::stoi(list[0])+std::stoi(list[1]);
            }
        }
    }
}

#endif //ILISP_UTIL_HPP
