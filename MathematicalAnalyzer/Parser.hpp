#pragma once


#include <optional>
#include <vector>
#include <iostream>
#include <memory>
#include "Tokenizer.hpp"
#include "Number.hpp"
#include "Node.hpp"
#include "Function.hpp"

using namespace functionNode;
#ifndef Parser_hpp
#define Parser_hpp

class Parser{
    std::shared_ptr<Node> start;
    std::vector<Token> src;
    std::vector<Function> functions;
    std::vector<std::string> variableNames;
    std::vector<Number> variableValues;
    
private:
    void resolveTree(std::shared_ptr<Node> base);
    std::shared_ptr<Node> applyDerivative(std::shared_ptr<Node> base);
    void parse(std::vector<Token> tokens, std::shared_ptr<Node> base);
    std::shared_ptr<Node> copyParseTree(std::shared_ptr<Node> base);
    std::shared_ptr<Node> simplfyTree(std::shared_ptr<Node> base);
    bool isTreeConstant(std::shared_ptr<Node> base);
    bool isDefinedFunction(std::string name);
    bool isDefinedVariable(std::string name);
    void findEveryVariable(std::shared_ptr<Node> base, std::vector<std::shared_ptr<Node>>& vars);
    std::shared_ptr<Node> changeEveryVariableWith(std::shared_ptr<Node> base, std::shared_ptr<Node> expr);
    void deleteTree(std::shared_ptr<Node> base);
public:
    Parser(){}
    std::shared_ptr<Node> parse();
    std::shared_ptr<Node> parse(std::vector<Token> tokens);
    void addFunction(std::string name, std::shared_ptr<Node> expr);
    void addVariable(std::string name, std::shared_ptr<Node> val);
    Parser(std::vector<Token> tokens);
    void showFunctionCalculation(std::string funName, const Number& num, mpfr_prec_t prec = 3);
};


#endif /* Parser_hpp */
