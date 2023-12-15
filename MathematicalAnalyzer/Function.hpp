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
public:
    Function(Node* src):src(src) {}
    double operator ()(double x){return fun(src,x);}
    
private:
    double fun(Node* base, double x);
};


#endif /* Function_hpp */
