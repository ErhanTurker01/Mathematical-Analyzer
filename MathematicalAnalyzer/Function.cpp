#include "Function.hpp"


/*Calculates function given a input*/
Number Function::operator()(const Number &x, std::shared_ptr<Node> base){
    if(base->nodeType == NodeType::expr){
        if(base->value.has_value()) return base->value.value();
        else return this->operator()(x,base->childs[0]);
    }
    else if(base->nodeType == NodeType::num) return base->value.value();
    else if(base->nodeType == NodeType::var) return x;
    else if(base->nodeType == NodeType::opr) {
        switch (base->oprType.value()) {
            case OprType::add:{
                Number num = this->operator()(x,base->childs[0]);
                num += this->operator()(x,base->childs[1]);
                return num;
            }
            case OprType::mult:{
                Number num = this->operator()(x,base->childs[0]);
                num *= this->operator()(x,base->childs[1]);
                return num;
            }
            case OprType::div:{
                Number num = this->operator()(x,base->childs[0]);
                num /= this->operator()(x,base->childs[1]);
                return num;
            }
            case OprType::sub:{
                Number num = this->operator()(x,base->childs[0]);
                num -= this->operator()(x,base->childs[1]);
                return num;
            }
            case OprType::pow:{
                Number num = this->operator()(x,base->childs[0]);
                num ^= this->operator()(x,base->childs[1]);
                return num;
            }
        }
    }
    else if(base->nodeType == NodeType::fun){
        switch (base->funType.value()) {
            case FunType::sin:{
                Number num = this->operator()(x,base->childs[0]);
                num.sinSelf();
                return num;
            }
            case FunType::cos:{
                Number num = this->operator()(x,base->childs[0]);
                num.cosSelf();
                return num;
            }
            case FunType::ln:{
                Number num = this->operator()(x,base->childs[0]);
                num.logSelf(*Number::e);
                return num;
            }
            case FunType::abs:{
                Number num = this->operator()(x,base->childs[0]);
                num.absSelf();
                return num;
            }
        }
    }
    else {
        return Number();
    }
}

Number Function::operator ()(const Number &x){
    return this->operator()(x, src);
}

std::shared_ptr<Node> Function::getSource(){
    return src;
}

const std::string& Function::getName(){
    return name;
}
