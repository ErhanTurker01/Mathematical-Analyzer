#pragma once


#include <optional>
#include <vector>
#include "Tokenizer.hpp"


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

typedef struct Node Node;
struct Node{
    NodeType nodeType;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
    std::optional<double> value;
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
    std::vector<double> variableValues;
    
private:
    void resolveTree(Node* base);
    Node* applyDerivative(Node* base);
    void Parse(std::vector<Token> tokens, Node* base);
    Node* copyParseTree(Node* base);

private:
    bool isDefinedFunction(std::string name) {return std::find(functionNames.begin(), functionNames.end(), name) != functionNames.end();}
    bool isDefinedVariable(std::string name) {return std::find(variableNames.begin(), variableNames.end(), name) != variableNames.end();}
    void changeEveryVariableWith(Node* base, Node* expr);
    void deleteTree(Node* base);
public:
    Parser(){}
    Node* parse(){
        Parse(src,start);
        return start->childs[0];
    }
    Node* parse(std::vector<Token> tokens){
        src = tokens;
        start = new Node({.nodeType = NodeType::expr});
        Parse(src,start);
        return start->childs[0];
    }
    void addFunction(std::string name, Node* expr) {functionNames.push_back(name);functionExprs.push_back(expr);}
    void addVariable(std::string name, double val) {variableNames.push_back(name);variableValues.push_back(val);}
    Parser(std::vector<Token> tokens):
    src(tokens),start(new Node({.nodeType = NodeType::expr})){}
    
};


#endif /* Parser_hpp */
