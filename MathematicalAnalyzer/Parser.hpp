#pragma once


#include <optional>
#include <vector>
#include <iostream>
#include "Tokenizer.hpp"
#include "Number.hpp"
#include "Node.hpp"
#include "Function.hpp"

using namespace functionNode;
#ifndef Parser_hpp
#define Parser_hpp

class Parser{
    Node *start;
    std::vector<Token> src;
    std::vector<Function> functions;
    std::vector<std::string> variableNames;
    std::vector<Number> variableValues;
    
private:
    void resolveTree(Node* base);
    Node* applyDerivative(Node* base);
    void parse(std::vector<Token> tokens, Node* base);
    Node* copyParseTree(Node* base);
    Node* simplfyTree(Node* base);
    bool isTreeConstant(Node* base);
    bool isDefinedFunction(std::string name);
    bool isDefinedVariable(std::string name);
    void findEveryVariable(Node* base, std::vector<Node*>& vars);
    Node* changeEveryVariableWith(Node* base, Node* expr);
    void deleteTree(Node* base);
public:
    Parser(){}
    Node* parse();
    Node* parse(std::vector<Token> tokens);
    void addFunction(std::string name, Node* expr);
    void addVariable(std::string name, Node* val);
    Parser(std::vector<Token> tokens);
    void showFunctionCalculation(std::string funName, const Number& num, mpfr_prec_t prec = 3);
};


#endif /* Parser_hpp */
