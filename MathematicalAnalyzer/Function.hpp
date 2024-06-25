#pragma once


#include <optional>
#include <vector>
#include <string>
#include "Node.hpp"
#include "Number.hpp"

#ifndef Function_hpp
#define Function_hpp

using namespace functionNode;

class Function{
    Node* src;
    std::string name;
    
private:
    Number operator ()(const Number &x, Node *base);
    
public:
    Function(Node* src, std::string name):src(src),name(name) {}
    Number operator ()(const Number &x);
    Node* getSource();
    const std::string& getName();
};


#endif /* Function_hpp */
