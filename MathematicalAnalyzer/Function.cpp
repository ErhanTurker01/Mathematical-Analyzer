#include "Function.hpp"


/*Calculates function given a input*/
Number Function::fun(Node *base, Number x){
    if(base->nodeType == NodeType::expr){
        if(base->value.has_value()) return base->value.value();
        else return fun(base->childs[0],x);
    }
    else if(base->nodeType == NodeType::num) return base->value.value();
    else if(base->nodeType == NodeType::var) return x;
    else if(base->nodeType == NodeType::opr) {
        switch (base->oprType.value()) {
            case OprType::add: return fun(base->childs[0],x) + fun(base->childs[1],x);
            case OprType::mult: return fun(base->childs[0],x) * fun(base->childs[1],x);
            case OprType::div: return fun(base->childs[0],x) / fun(base->childs[1],x);
            case OprType::sub: return fun(base->childs[0],x) - fun(base->childs[1],x);
            case OprType::pow: return fun(base->childs[0],x) ^ fun(base->childs[1],x);
        }
    }
    else if(base->nodeType == NodeType::fun){
        switch (base->funType.value()) {
            case FunType::sin:return fun(base->childs[0],x).sin(); break;
            case FunType::cos:return fun(base->childs[0],x).cos();break;
            case FunType::ln:return fun(base->childs[0],x).log(*Number::e);break;
            case FunType::abs:return fun(base->childs[0],x).abs();break;
        }
    }
    else {
        return Number();
    }
}
