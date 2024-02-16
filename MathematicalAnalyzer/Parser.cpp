#include "Parser.hpp"


using namespace functionNode;
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
            else if(!(prc-plc) && token.type == TokenType::der){
                base->childs.push_back(new Node({.nodeType = NodeType::der}));
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
    
    /* This part handles exponential expressions */
    for(int i=1;i<childs.size()-1;i++){
        if(childs[i]->nodeType == NodeType::opr && (childs[i]->oprType == OprType::pow)){
            if(childs[i+1]->nodeType != NodeType::opr && childs[i-1]->nodeType != NodeType::opr){
                Node* expr = new Node({.nodeType = NodeType::expr});
                Node* opr = childs[i];
                Node* base = childs[i-1];
                Node* power = childs[i+1];
                opr->childs.push_back(base);
                opr->childs.push_back(power);
                expr->childs.push_back(opr);
                childs.insert(childs.begin()+i-1, expr);
                childs.erase(childs.begin()+i,childs.begin()+i+3);
                
            }
            else{
                throw "Operation needs expression";
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
    
    for (int i=0; i<childs.size()-1; i++) {
        if (childs[i]->nodeType == NodeType::der) {
            Node* der = childs[i];
            childs.erase(childs.begin()+i);
            delete der;
            childs[i] = applyDerivative(childs[i]);
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
            delete childs[0];
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
            delete childs[0];
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
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::add) {
            delete childs[0];
            childs.erase(childs.begin());
        }
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::sub){
            if(childs[1]->nodeType == NodeType::num){
                childs[1]->value.value() *=-1;
                delete childs[0];
                childs.erase(childs.begin());
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

Node* ParseTree::applyDerivative(Node* base){
    std::vector<Node*>& childs = base->childs;
    if(base->nodeType == NodeType::expr) return new Node(*applyDerivative(childs[0]));
    else if(base->nodeType == NodeType::num) return new Node({.nodeType = NodeType::num, .value = 0});
    else if(base->nodeType == NodeType::var) return new Node({.nodeType = NodeType::num, .value = 1});
    else if(base->nodeType == NodeType::opr){
        switch (base->oprType.value()) {
            case OprType::add:
            case OprType::sub:
            {
                Node* opr = new Node({.nodeType = NodeType::opr, .oprType = base->oprType.value()});
                opr->childs.push_back(applyDerivative(childs[0]));
                opr->childs.push_back(applyDerivative(childs[1]));
                return opr;
            }
            case OprType::mult:
            {
                Node* opr = new Node({.nodeType = NodeType::opr, .oprType = OprType::add});
                Node *expr1 = new Node({.nodeType = NodeType::expr}),
                *expr2 = new Node({.nodeType = NodeType::expr}),
                *mult1 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *mult2 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *left = new Node(*childs[0]),
                *right = new Node(*childs[1]),
                *derLeft = applyDerivative(left),
                *derRight = applyDerivative(right);
                opr->childs.push_back(expr1);
                opr->childs.push_back(expr2);
                expr1->childs.push_back(mult1);
                expr2->childs.push_back(mult2);
                mult1->childs.push_back(left);
                mult1->childs.push_back(derRight);
                mult2->childs.push_back(derLeft);
                mult2->childs.push_back(right);
                return opr;
            }
            case OprType::div:
            {
                Node* numerator = new Node({.nodeType = NodeType::expr});
                {
                    Node* opr = new Node({.nodeType = NodeType::opr, .oprType = OprType::sub});
                    Node *expr1 = new Node({.nodeType = NodeType::expr}),
                    *expr2 = new Node({.nodeType = NodeType::expr}),
                    *mult1 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                    *mult2 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                    *left = new Node(*childs[0]),
                    *right = new Node(*childs[1]),
                    *derLeft = applyDerivative(left),
                    *derRight = applyDerivative(right);
                    opr->childs.push_back(expr2);
                    opr->childs.push_back(expr1);
                    expr1->childs.push_back(mult1);
                    expr2->childs.push_back(mult2);
                    mult1->childs.push_back(left);
                    mult1->childs.push_back(derRight);
                    mult2->childs.push_back(derLeft);
                    mult2->childs.push_back(right);
                    numerator->childs.push_back(opr);
                }
                Node* denominator = new Node({.nodeType = NodeType::expr});
                denominator->childs.push_back(new Node({.nodeType = NodeType::opr, OprType::mult}));
                denominator->childs[0]->childs.push_back(new Node(*childs[1]));
                denominator->childs[0]->childs.push_back(new Node(*childs[1]));
                Node* expr = new Node({.nodeType = NodeType::expr});
                expr->childs.push_back(new Node({.nodeType = NodeType::opr, .oprType = OprType::div}));
                expr->childs[0]->childs.push_back(numerator);
                expr->childs[0]->childs.push_back(denominator);
                return expr;
            }
            case OprType::pow:
                Node* f1 = new Node(*childs[0]),
                *f2 = new Node(*childs[0]),
                *g1 = new Node(*childs[1]),
                *fd = applyDerivative(childs[0]),
                *gd = applyDerivative(childs[1]),
                *add = new Node({.nodeType = NodeType::opr, .oprType = OprType::add}),
                *mult1 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *mult2 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *mult3 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *div = new Node({.nodeType = NodeType::opr, .oprType = OprType::div}),
                *absf = new Node({.nodeType = NodeType::fun, .funType = FunType::abs}),
                *lnf = new Node({.nodeType = NodeType::fun, .funType = FunType::ln}),
                *fdDfexpr = new Node({.nodeType = NodeType::expr}),
                *fdtgexpr = new Node({.nodeType = NodeType::expr}),
                *absfexpr = new Node({.nodeType = NodeType::expr}),
                *lnexpr = new Node({.nodeType = NodeType::expr}),
                *absexpr = new Node({.nodeType = NodeType::expr}),
                *mlngdexpr = new Node({.nodeType = NodeType::expr}),
                *sumexpr = new Node({.nodeType = NodeType::expr}),
                *all = new Node({.nodeType = NodeType::expr});
                sumexpr->childs.push_back(add);
                add->childs.push_back(fdtgexpr);
                fdtgexpr->childs.push_back(mult1);
                mult1->childs.push_back(g1);
                mult1->childs.push_back(fdDfexpr);
                fdDfexpr->childs.push_back(div);
                div->childs.push_back(fd);
                div->childs.push_back(f1);
                add->childs.push_back(mlngdexpr);
                mlngdexpr->childs.push_back(mult2);
                mult2->childs.push_back(gd);
                mult2->childs.push_back(lnexpr);
                lnexpr->childs.push_back(lnf);
                lnf->childs.push_back(absfexpr);
                absfexpr->childs.push_back(absf);
                absf->childs.push_back(absexpr);
                absexpr->childs.push_back(f2);
                mult3->childs.push_back(base);
                mult3->childs.push_back(sumexpr);
                all->childs.push_back(mult3);
                return all;
        }
    }
    else if (base->nodeType == NodeType::fun){
        switch (base->funType.value()) {
            case FunType::cos:
            {
                Node* sin = new Node({.nodeType = NodeType::fun, .funType = FunType::sin}),
                *d = applyDerivative(base->childs[0]),
                *mult1 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *mult2 = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *expr1 = new Node({.nodeType = NodeType::expr}),
                *expr2 = new Node({.nodeType = NodeType::expr}),
                *expr3 = new Node({.nodeType = NodeType::expr}),
                *one = new Node({.nodeType =  NodeType::num, .value = -1});
                expr1->childs.push_back(mult1);
                mult1->childs.push_back(one);
                mult1->childs.push_back(expr2);
                expr2->childs.push_back(mult2);
                mult2->childs.push_back(d);
                mult2->childs.push_back(expr3);
                expr3->childs.push_back(sin);
                sin->childs.push_back(new Node(*base->childs[0]));
                return expr1;
            }
            case FunType::sin:
            {
                Node* cos = new Node({.nodeType = NodeType::fun, .funType = FunType::cos}),
                *d = applyDerivative(base->childs[0]),
                *mult = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *expr1 = new Node({.nodeType = NodeType::expr}),
                *expr2 = new Node({.nodeType = NodeType::expr});
                expr1->childs.push_back(mult);
                mult->childs.push_back(d);
                mult->childs.push_back(expr2);
                expr2->childs.push_back(cos);
                cos->childs.push_back(new Node(*base->childs[0]));
                return expr1;
            }
            case FunType::ln:
            {
                Node* expr = new Node({.nodeType = NodeType::expr}),
                *div = new Node({.nodeType = NodeType::opr, .oprType = OprType::div}),
                *d = applyDerivative(base->childs[0]);
                expr->childs.push_back(div);
                div->childs.push_back(d);
                div->childs.push_back(new Node(*base->childs[0]));
                return expr;
            }
            case FunType::abs:
            {
                Node* abs = new Node({.nodeType = NodeType::fun, .funType = FunType::abs}),
                *expr1 = new Node({.nodeType = NodeType::expr}),
                *expr2 = new Node({.nodeType = NodeType::expr}),
                *expr3 = new Node({.nodeType = NodeType::expr}),
                *expr4 = new Node({.nodeType = NodeType::expr}),
                *expr5 = new Node({.nodeType = NodeType::expr}),
                *mult = new Node({.nodeType = NodeType::opr, .oprType = OprType::mult}),
                *div = new Node({.nodeType = NodeType::opr, .oprType = OprType::div}),
                *f1 = new Node(*base->childs[0]),
                *f2 = new Node(*base->childs[0]),
                *d = applyDerivative(base->childs[0]);
                expr1->childs.push_back(mult);
                mult->childs.push_back(d);
                mult->childs.push_back(expr2);
                expr2->childs.push_back(div);
                div->childs.push_back(expr3);
                div->childs.push_back(expr4);
                expr3->childs.push_back(f1);
                expr4->childs.push_back(abs);
                abs->childs.push_back(expr5);
                expr5->childs.push_back(f2);
                return expr1;
            }
        }
    }
    return nullptr;
}
