#include "Function.hpp"


/*Calculates function given a input*/
Number Function::fun(Node *base, const Number &x){
    if(base->nodeType == NodeType::expr){
        if(base->value.has_value()) return base->value.value();
        else return fun(base->childs[0],x);
    }
    else if(base->nodeType == NodeType::num) return base->value.value();
    else if(base->nodeType == NodeType::var) return x;
    else if(base->nodeType == NodeType::opr) {
        switch (base->oprType.value()) {
            case OprType::add:{
                Number num = fun(base->childs[0],x);
                num += fun(base->childs[1],x);
                return num;
            }
            case OprType::mult:{
                Number num = fun(base->childs[0],x);
                num *= fun(base->childs[1],x);
                return num;
            }
            case OprType::div:{
                Number num = fun(base->childs[0],x);
                num /= fun(base->childs[1],x);
                return num;
            }
            case OprType::sub:{
                Number num = fun(base->childs[0],x);
                num -= fun(base->childs[1],x);
                return num;
            }
            case OprType::pow:{
                Number num = fun(base->childs[0],x);
                num ^= fun(base->childs[1],x);
                return num;
            }
        }
    }
    else if(base->nodeType == NodeType::fun){
        switch (base->funType.value()) {
            case FunType::sin:{
                Number num = fun(base->childs[0],x);
                num.sinSelf();
                return num;
            }
            case FunType::cos:{
                Number num = fun(base->childs[0],x);
                num.cosSelf();
                return num;
            }
            case FunType::ln:{
                Number num = fun(base->childs[0],x);
                num.logSelf(*Number::e);
                return num;
            }
            case FunType::abs:{
                Number num = fun(base->childs[0],x);
                num.absSelf();
                return num;
            }
        }
    }
    else {
        return Number();
    }
}
