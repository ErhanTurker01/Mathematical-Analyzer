#pragma once


#include <optional>
#include <vector>
#include <string>
#include "Parser.hpp"

#ifndef Function_hpp
#define Function_hpp

using namespace functionNode;

class Function{
    Node* src;
    std::string name;
public:
    Function(Node* src, std::string name):src(src),name(name) {}
    double operator ()(double x){return fun(src,x);}
    std::string getName(){return name;}
    
private:
    double fun(Node* base, double x);
};


#endif /* Function_hpp */
