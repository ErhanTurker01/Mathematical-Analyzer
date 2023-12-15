#include "Parser.hpp"



/*From tokens creates a tree with necessary nodes*/
void ParseTree::parse(std::vector<Token> tokens, Node *base){
    {
        int prc = 0, plc = 0,begin = 0;
        for(int end=0;end<tokens.size();end++){
            auto token = tokens[end];
            if(token.type == TokenType::pl) plc++;
            if(token.type == TokenType::pr) prc++;
            if(prc == plc && (plc+prc)){
                base->childs.push_back(new Node({.nodeType = NodeType::expr}));
                parse(std::vector<Token>(tokens.begin() + begin + 1, tokens.begin() + end), base->childs.back());
                begin = end + 1;
                prc = 0;
                plc = 0;
            }
            else if(!(prc-plc) && token.type == TokenType::opr){
                base->childs.push_back(new Node({.nodeType = NodeType::opr, .oprType = token.oprType}));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::num){
                base->childs.push_back(new Node({.nodeType = NodeType::num, .value = std::stod(token.value.value())}));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::var){
                base->childs.push_back(new Node({.nodeType = NodeType::var}));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::fun){
                base->childs.push_back(new Node({.nodeType = NodeType::fun, .funType = token.funType}));
                begin++;
            }
        }
    }
    resolveTree(base);
}

/*Result after parse function is not binary tree. This function takes that tree and makes it a binary tree for easier implementation*/
void ParseTree::resolveTree(Node *base){
    /*reference for later use*/
    std::vector<Node*>& childs = base->childs;
    for(auto node:childs) if(node->nodeType == NodeType::expr) resolveTree(node);
    
    /* This part handles funcitons */
    for(int i=0;i<childs.size();i++){
        if(childs[i]->nodeType == NodeType::fun){
            if(childs[i+1] != NULL && childs[i+1]->nodeType == NodeType::expr){
                Node* fun = childs[i];
                Node* funexpr = childs[i+1];
                Node* expr = new Node({.nodeType = NodeType::expr});
                fun->childs.push_back(funexpr);
                childs.erase(childs.begin()+i, childs.begin()+i+2);
                childs.insert(childs.begin()+i, expr);
                expr->childs.push_back(fun);
            }
            else if(childs[0]->childs.size() == 0){
                throw "Function need expression";
            }
        }
    }
    
    /*This part adds mult node where necessary*/
    for (int i=0; i<childs.size()-1; i++) {
        if(childs[i]->nodeType == NodeType::expr){
            if(childs[i+1]->nodeType == NodeType::expr || childs[i+1]->nodeType == NodeType::num || childs[i+1]->nodeType == NodeType::var){
                childs.insert(childs.begin()+ ++i, new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
        else if(childs[i]->nodeType == NodeType::num){
            if(childs[i+1]->nodeType == NodeType::expr || childs[i+1]->nodeType == NodeType::var){
                childs.insert(childs.begin()+ ++i, new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
        else if(childs[i]->nodeType == NodeType::var){
            if(childs[i+1]->nodeType == NodeType::expr){
                childs.insert(childs.begin()+ ++i, new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
    }
    
    /*Rest of the function handles operations and recursively makes tree a binary tree*/
    if(childs.size() == 3){
        if(childs[0]->nodeType != NodeType::opr && childs[1]->oprType.has_value()){
            Node* opr = childs[1];
            opr->childs.push_back(base->childs[0]);
            opr->childs.push_back(base->childs[2]);
            childs.pop_back();
            childs.erase(base->childs.begin());
            return;
        }
        else if(childs[0]->oprType == OprType::sub){
            childs.insert(childs.begin(), new Node({.nodeType = NodeType::num, .value = 0}));
            resolveTree(base);
            return;
        }
        else if(childs[0]->oprType == OprType::add){
            childs.erase(childs.begin());
            resolveTree(base);
            return;
        }
    }
    
    else if(childs.size() == 2){
        if(childs[0]->nodeType != NodeType::opr){
            if(childs[0]->nodeType == NodeType::fun){
                childs[0]->childs.push_back(childs[1]);
                childs.pop_back();
            }
            else{
                childs.insert(childs.begin()+1, new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}));
                resolveTree(base);
                return;
            }
        }
        else if(childs[0]->oprType == OprType::sub){
            childs.insert(childs.begin(), new Node({.nodeType = NodeType::num, .value = 0}));
            resolveTree(base);
            return;
        }
        else if(childs[0]->oprType == OprType::add){
            childs.erase(childs.begin());
            resolveTree(base);
            return;
        }
        
    }
    else if(childs.size() == 1) {
        if(childs[0]->nodeType == NodeType::fun && !childs[0]->childs.size()){
            throw "Needs expession";
        }
        return;
    }
    else if(childs.size() == 0){
        return;
    }
    
    
    else if(childs.size() > 3){
        if(childs[0]->nodeType == NodeType::opr && (childs[0]->oprType == OprType::mult || childs[0]->oprType == OprType::div)) throw "Operation needs expression";
        if(childs.back()->nodeType == NodeType::opr) throw "Operation needs expression";
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::add) childs.erase(childs.begin());
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::sub){
            if(childs[1]->nodeType == NodeType::num){
                childs[1]->value.value() *=-1;
                childs.erase(childs.begin());
                childs.insert(childs.begin(), new Node({.nodeType = NodeType::opr, .oprType = OprType::add}));
            }
            else if(childs[1]->nodeType == NodeType::expr){
                childs[1]->childs.push_back(childs[0]);
                childs[0]->childs.push_back(new Node({.nodeType = NodeType::num, .value = 0}));
                childs[0]->childs.push_back(childs[1]->childs[0]);
                childs.erase(childs.begin());
            }
        }
        for(int i=1;i<childs.size()-1;i++){
            if(childs[i]->nodeType == NodeType::opr && (childs[i]->oprType == OprType::mult || childs[i]->oprType == OprType::div)){
                if(childs[i+1]->nodeType != NodeType::opr && childs[i-1]->nodeType != NodeType::opr){
                    Node* expr = new Node({.nodeType = NodeType::expr});
                    for(int j=-1;j<=1;j++) expr->childs.push_back(childs[i+j]);
                    childs.insert(childs.begin()+i-1, expr);
                    childs.erase(childs.begin()+i,childs.begin()+i+3);
                    resolveTree(expr);
                    i--;
                }
                else{
                    throw "Operation needs expression";
                }
            }
        }
        for(int i=1;i<childs.size()-1;i++){
            if(childs[i+1]->nodeType != NodeType::opr && childs[i-1]->nodeType != NodeType::opr){
                Node* expr = new Node({.nodeType = NodeType::expr});
                for(int j=-1;j<=1;j++) expr->childs.push_back(childs[i+j]);
                childs.insert(childs.begin()+i-1, expr);
                childs.erase(childs.begin()+i,childs.begin()+i+3);
                resolveTree(expr);
                i--;
            }
            else{
                throw "Operation needs expression";
            }
        }
        if(childs.size() != 1) resolveTree(base);
    }
}
