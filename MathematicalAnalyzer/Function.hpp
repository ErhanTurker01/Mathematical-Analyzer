#pragma once


#include <optional>
#include <vector>
#include <string>
#include <memory>
#include "Node.hpp"
#include "Number.hpp"

#ifndef Function_hpp
#define Function_hpp

using namespace functionNode;

class Variable{
    Number value;
    std::string name;
public:
    Variable(std::string name, const Number& num);
    std::string getName() const;
    const Number& getValue() const;
    
};

class Function{
    std::shared_ptr<Node> src;
    std::string name;
    
private:
    Number operator ()(const Number &x, std::shared_ptr<Node> base);
    
public:
    Function(std::shared_ptr<Node> src, std::string name):src(src),name(name) {}
    Number operator ()(const Number &x);
    std::shared_ptr<Node> getSource();
    const std::string& getName();
};


#endif /* Function_hpp */
