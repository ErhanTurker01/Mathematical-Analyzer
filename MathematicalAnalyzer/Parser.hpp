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
    fun
};

typedef struct Node Node;
struct Node{
    NodeType nodeType;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
    std::optional<double> value;
    std::vector<Node*> childs;
};
}

using namespace functionNode;
#ifndef Parser_hpp
#define Parser_hpp

class ParseTree{
    Node *start;
    std::vector<Token> src;
    
private:
    void resolveTree(Node* base);

public:
    void parse(std::vector<Token> tokens, Node* base);
    
public:
    Node* Parse(){
        parse(src,start);
        return start->childs[0];
    }
    
    ParseTree(std::vector<Token> tokens):
    src(std::move(tokens)),start(new Node({.nodeType = NodeType::expr})){}
    
};


#endif /* Parser_hpp */
