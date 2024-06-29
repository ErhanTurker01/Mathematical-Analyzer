#include "Parser.hpp"
#include "Function.hpp"


using namespace functionNode;
/*From tokens creates a tree with necessary nodes*/
void Parser::parse(std::vector<Token> tokens, std::shared_ptr<Node> base){
    if(tokens.size() == 0) throw "No expression!";
    {
        int prc = 0, plc = 0,begin = 0;
        for(int end=0;end<tokens.size();end++){
            auto token = tokens[end];
            if(token.type == TokenType::pl) plc++;
            if(token.type == TokenType::pr) prc++;
            if(token.type == TokenType::other){
                if (!isDefinedFunction(token.value.value())) {
                    if(!isDefinedVariable(token.value.value())){
                        throw "Undifined Symbols!";
                    }
                }
            }
            if(prc == plc && (plc+prc)){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::expr})));
                parse(std::vector<Token>(tokens.begin() + begin + 1, tokens.begin() + end), base->childs.back());
                begin = end + 1;
                prc = 0;
                plc = 0;
            }
            else if(!(prc-plc) && token.type == TokenType::opr){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = token.oprType})));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::num){
                std::shared_ptr<Node> node = std::make_shared<Node>();
                node->nodeType = NodeType::num;
                node->value = Number::getFromString(token.value.value());
                base->childs.push_back(node);
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::var){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::var})));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::fun){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::fun, .funType = token.funType})));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::der){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::der})));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::other && isDefinedFunction(token.value.value())){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::userFun, .name = token.value.value()})));
                begin++;
            }
            else if(!(prc-plc) && token.type == TokenType::other && isDefinedVariable(token.value.value())){
                base->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::userNum, .name = token.value.value()})));
                begin++;
            }
        }
    }
    resolveTree(base);
}

/*Result after parse function is not binary tree. This function takes that tree and makes it a binary tree for easier implementation*/
void Parser::resolveTree(std::shared_ptr<Node> base){
    /*reference for later use*/
    std::vector<std::shared_ptr<Node>>& childs = base->childs;
    
    for(int i=0;i<childs.size();i++){
        if (childs[i]->nodeType == NodeType::userNum) {
            for (auto& var : variables) {
                if(var.getName() == childs[i]->name){
                    childs[i]->value = var.getValue();
                    childs[i]->nodeType = NodeType::num;
                }
            }
        }
        else if(childs[i]->nodeType == NodeType::userFun){
            int funIndex;
            for (funIndex=0; childs[i]->name.value() != functions[funIndex].getName(); funIndex++);
            if(i+1<childs.size()){
                childs[i] = changeEveryVariableWith(copyParseTree(functions[funIndex].getSource()), childs[i+1]);
            }
            else throw "NO EXPR";
            childs.erase(childs.begin()+ ++i);
        }
    }
    
    for(auto node:childs) if(node->nodeType == NodeType::expr) resolveTree(node);
    
    /* This part handles functions */
    for(int i=0;i<childs.size();i++){
        if(childs[i]->nodeType == NodeType::fun){
            if(childs.size() > i + 1 && childs[i+1]->nodeType == NodeType::expr){
                std::shared_ptr<Node> fun = childs[i];
                std::shared_ptr<Node> funexpr = childs[i+1];
                std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
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
                std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
                std::shared_ptr<Node> opr = childs[i];
                std::shared_ptr<Node> base = childs[i-1];
                std::shared_ptr<Node> power = childs[i+1];
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
                childs.insert(childs.begin()+ ++i, std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
        else if(childs[i]->nodeType == NodeType::num){
            if(childs[i+1]->nodeType == NodeType::expr || childs[i+1]->nodeType == NodeType::var){
                childs.insert(childs.begin()+ ++i, std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
        else if(childs[i]->nodeType == NodeType::var){
            if(childs[i+1]->nodeType == NodeType::expr){
                childs.insert(childs.begin()+ ++i, std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})));
            }
            else if(childs[i+1]->nodeType == NodeType::opr){}
            else{
                throw "what";
            }
        }
    }
    
    for (int i=0; i<childs.size()-1; i++) {
        if (childs[i]->nodeType == NodeType::der) {
            std::shared_ptr<Node> der = childs[i];
            childs.erase(childs.begin()+i);
            childs[i] = applyDerivative(childs[i]);
        }
    }
    
    /*Rest of the function handles operations and recursively makes tree a binary tree*/
    if(childs.size() == 3){
        if(childs[0]->nodeType != NodeType::opr && childs[1]->oprType.has_value()){
            std::shared_ptr<Node> opr = childs[1];
            opr->childs.push_back(base->childs[0]);
            opr->childs.push_back(base->childs[2]);
            childs.pop_back();
            childs.erase(base->childs.begin());
            return;
        }
        else if(childs[0]->oprType == OprType::sub){
            childs.insert(childs.begin(), std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = Number()})));
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
                childs.insert(childs.begin()+1, std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})));
                resolveTree(base);
                return;
            }
        }
        else if(childs[0]->oprType == OprType::sub){
            childs.insert(childs.begin(), std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = Number()})));
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
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::add) {
            childs.erase(childs.begin());
        }
        if(childs[0]->nodeType == NodeType::opr && childs[0]->oprType == OprType::sub){
            if(childs[1]->nodeType == NodeType::num){
                childs[1]->value.value() *= *Number::negOne;
                childs.erase(childs.begin());
            }
            else if(childs[1]->nodeType == NodeType::expr){
                childs[1]->childs.push_back(childs[0]);
                childs[0]->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = Number()})));
                childs[0]->childs.push_back(childs[1]->childs[0]);
                childs.erase(childs.begin());
            }
        }
        for(int i=1;i<childs.size()-1;i++){
            if(childs[i]->nodeType == NodeType::opr && (childs[i]->oprType == OprType::mult || childs[i]->oprType == OprType::div)){
                if(childs[i+1]->nodeType != NodeType::opr && childs[i-1]->nodeType != NodeType::opr){
                    std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
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
                std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
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
    simplfyTree(base);
}

std::shared_ptr<Node> Parser::parse(std::vector<Token> tokens){
    src = tokens;
    start = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
    parse(src,start);
    return start->childs[0];
}

std::shared_ptr<Node> Parser::parse(){
    parse(src,start);
    return start->childs[0];
}

void Parser::addFunction(std::string name, std::shared_ptr<Node> expr){
    functions.push_back(Function(expr, name));
}

void Parser::addVariable(std::string name, std::shared_ptr<Node> val){
    variableNames.push_back(name);variableValues.push_back(simplfyTree(val)->value.value());
}

Parser::Parser(std::vector<Token> tokens):
    src(tokens), start(new Node({.nodeType = NodeType::expr})){
    
}

std::shared_ptr<Node> Parser::simplfyTree(std::shared_ptr<Node> base){
    if(isTreeConstant(base)){
        Function fun(base,"");
        base = std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = fun(Number())}));
    }
    return base;
}

bool Parser::isTreeConstant(std::shared_ptr<Node> base){
    if (base->nodeType == NodeType::var) return false;
    for (std::shared_ptr<Node> child : base->childs) if(!isTreeConstant(child)) return false;
    return true;
}

std::shared_ptr<Node> Parser::applyDerivative(std::shared_ptr<Node> base){
    std::vector<std::shared_ptr<Node>>& childs = base->childs;
    if(base->nodeType == NodeType::expr) return std::make_shared<Node>(Node(*applyDerivative(childs[0])));
    else if(base->nodeType == NodeType::num) return std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = Number()}));
    else if(base->nodeType == NodeType::var) return std::make_shared<Node>(Node({.nodeType = NodeType::num, .value = *Number::one}));
    else if(base->nodeType == NodeType::opr){
        switch (base->oprType.value()) {
            case OprType::add:
            case OprType::sub:
            {
                std::shared_ptr<Node> opr = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = base->oprType.value()}));
                opr->childs.push_back(applyDerivative(childs[0]));
                opr->childs.push_back(applyDerivative(childs[1]));
                return opr;
            }
            case OprType::mult:
            {
                std::shared_ptr<Node> opr = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::add}));
                std::shared_ptr<Node> expr1 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr2 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                mult1 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                mult2 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                left = std::make_shared<Node>(Node(*childs[0])),
                right = std::make_shared<Node>(Node(*childs[1])),
                derLeft = applyDerivative(left),
                derRight = applyDerivative(right);
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
                std::shared_ptr<Node> numerator = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
                {
                    std::shared_ptr<Node> opr = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::sub}));
                    std::shared_ptr<Node> expr1 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                    expr2 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                    mult1 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                    mult2 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                    left =  std::make_shared<Node>(Node(*childs[0])),
                    right = std::make_shared<Node>(Node(*childs[1])),
                    derLeft = applyDerivative(left),
                    derRight = applyDerivative(right);
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
                std::shared_ptr<Node> denominator = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
                denominator->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::opr, OprType::mult})));
                denominator->childs[0]->childs.push_back(std::make_shared<Node>(Node(*childs[1])));
                denominator->childs[0]->childs.push_back(std::make_shared<Node>(Node(*childs[1])));
                std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
                expr->childs.push_back(std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::div})));
                expr->childs[0]->childs.push_back(numerator);
                expr->childs[0]->childs.push_back(denominator);
                return expr;
            }
            case OprType::pow:
                std::shared_ptr<Node> f1 = std::make_shared<Node>(Node(*childs[0])),
                f2 = std::make_shared<Node>(Node(*childs[0])),
                g1 = std::make_shared<Node>(Node(*childs[1])),
                fd = applyDerivative(childs[0]),
                gd = applyDerivative(childs[1]),
                add = std::make_shared<Node>( Node({.nodeType = NodeType::opr, .oprType = OprType::add})),
                mult1 = std::make_shared<Node>( Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                mult2 = std::make_shared<Node>( Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                mult3 = std::make_shared<Node>( Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                div = std::make_shared<Node>( Node({.nodeType = NodeType::opr, .oprType = OprType::div})),
                absf = std::make_shared<Node>( Node({.nodeType = NodeType::fun, .funType = FunType::abs})),
                lnf = std::make_shared<Node>( Node({.nodeType = NodeType::fun, .funType = FunType::ln})),
                fdDfexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                fdtgexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                absfexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                lnexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                absexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                mlngdexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                sumexpr = std::make_shared<Node>( Node({.nodeType = NodeType::expr})),
                all = std::make_shared<Node>( Node({.nodeType = NodeType::expr}));
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
                std::shared_ptr<Node> sin = std::make_shared<Node>(Node({.nodeType = NodeType::fun, .funType = FunType::sin})),
                d = applyDerivative(base->childs[0]),
                mult1 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                mult2 = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                expr1 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr2 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr3 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                one = std::make_shared<Node>(Node({.nodeType =  NodeType::num, .value = *Number::negOne}));
                expr1->childs.push_back(mult1);
                mult1->childs.push_back(one);
                mult1->childs.push_back(expr2);
                expr2->childs.push_back(mult2);
                mult2->childs.push_back(d);
                mult2->childs.push_back(expr3);
                expr3->childs.push_back(sin);
                sin->childs.push_back(std::make_shared<Node>(Node(*base->childs[0])));
                return expr1;
            }
            case FunType::sin:
            {
                std::shared_ptr<Node> cos = std::make_shared<Node>(Node({.nodeType = NodeType::fun, .funType = FunType::cos})),
                d = applyDerivative(base->childs[0]),
                mult =  std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                expr1 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr2 = std::make_shared<Node>(Node({.nodeType = NodeType::expr}));
                expr1->childs.push_back(mult);
                mult->childs.push_back(d);
                mult->childs.push_back(expr2);
                expr2->childs.push_back(cos);
                cos->childs.push_back(std::make_shared<Node>(Node(*base->childs[0])));
                return expr1;
            }
            case FunType::ln:
            {
                std::shared_ptr<Node> expr = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                div = std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::div})),
                d = applyDerivative(base->childs[0]);
                expr->childs.push_back(div);
                div->childs.push_back(d);
                div->childs.push_back(std::make_shared<Node>(Node(*base->childs[0])));
                return expr;
            }
            case FunType::abs:
            {
                std::shared_ptr<Node> abs = std::make_shared<Node>(Node({.nodeType = NodeType::fun, .funType = FunType::abs})),
                expr1 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr2 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr3 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr4 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                expr5 = std::make_shared<Node>(Node({.nodeType = NodeType::expr})),
                mult =  std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::mult})),
                div =   std::make_shared<Node>(Node({.nodeType = NodeType::opr, .oprType = OprType::div})),
                f1 =    std::make_shared<Node>(Node(*base->childs[0])),
                f2 =    std::make_shared<Node>(Node(*base->childs[0])),
                d = applyDerivative(base->childs[0]);
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

std::shared_ptr<Node> Parser::copyParseTree(std::shared_ptr<Node> base){
    std::shared_ptr<Node> newBase = std::make_shared<Node>(Node(*base));
    for(uint8_t i=0;i<base->childs.size();i++) newBase->childs[i] = (std::make_shared<Node>(Node(*copyParseTree(base->childs[i]))));
    return newBase;
}

void Parser::findEveryVariable(std::shared_ptr<Node> base, std::vector<std::shared_ptr<Node>>& vars){
    if (base->nodeType == NodeType::var) vars.push_back(base);
    for (std::shared_ptr<Node> &child : base->childs) findEveryVariable(child, vars);
}

std::shared_ptr<Node> Parser::changeEveryVariableWith(std::shared_ptr<Node> base, std::shared_ptr<Node> expr){
    std::vector<std::shared_ptr<Node>> vars;
    findEveryVariable(base, vars);
    for (std::shared_ptr<Node> var : vars) *var = *copyParseTree(expr);
    return base;
}

void Parser::deleteTree(std::shared_ptr<Node> base){
    for (std::shared_ptr<Node> child : base->childs) deleteTree(child);
}

bool Parser::isDefinedFunction(std::string name){
    for (auto& fun : functions) {
        if(fun.getName() == name) return true;
    }
    return false;
}

bool Parser::isDefinedVariable(std::string name){
    for (auto& var : variables) {
        if(var.getName() == name) return true;
    }
    return false;
}

void Parser::showFunctionCalculation(std::string funName, const Number& num, mpfr_prec_t prec){
    for (Function& fun : functions){
        if(fun.getName() == funName){
            std::cout << funName << "(" << std::flush;
            num.print();
            std::cout << ") = " << std::flush;
            fun(num).print();
            std::cout << std::endl;
        }
    }
}
