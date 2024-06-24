#pragma once


#include <optional>
#include <vector>
#include <string>
#include "Parser.hpp"
#include "Number.hpp"

#ifndef Function_hpp
#define Function_hpp

using namespace functionNode;

class Function{
    Node* src;
    std::string name;
public:
    Function(Node* src, std::string name):src(src),name(name) {}
    Number operator ()(Number x){return fun(src,x);}
    std::string getName(){return name;}
    
private:
    Number fun(Node* base, Number x);
};


#endif /* Function_hpp */
