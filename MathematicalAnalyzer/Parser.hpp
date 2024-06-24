#pragma once


#include <optional>
#include <vector>
#include "Tokenizer.hpp"
#include "Number.hpp"

namespace functionNode {

enum class NodeType{
    expr,
    num,
    opr,
    var,
    fun,
    der,
    userNum,
    userFun
};

struct Node{
    NodeType nodeType;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
    std::optional<Number> value;
    std::optional<std::string> name;
    std::vector<Node*> childs;
};
}

using namespace functionNode;
#ifndef Parser_hpp
#define Parser_hpp

class Parser{
    Node *start;
    std::vector<Token> src;
    std::vector<std::string> functionNames;
    std::vector<Node*> functionExprs;
    std::vector<std::string> variableNames;
    std::vector<Number> variableValues;
    
private:
    void resolveTree(Node* base);
    Node* applyDerivative(Node* base);
    void parse(std::vector<Token> tokens, Node* base);
    Node* copyParseTree(Node* base);
    Node* simplfyTree(Node* base);
    bool isTreeConstant(Node* base);

private:
    bool isDefinedFunction(std::string name) {return std::find(functionNames.begin(), functionNames.end(), name) != functionNames.end();}
    bool isDefinedVariable(std::string name) {return std::find(variableNames.begin(), variableNames.end(), name) != variableNames.end();}
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
    
};


#endif /* Parser_hpp */
