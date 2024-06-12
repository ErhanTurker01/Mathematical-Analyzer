//
//  Number.cpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 4/23/24.
//

#include <cstdint>
#include <cmath>
#include <iostream>
#include "Number.hpp"

void Number::init (const mpfr_prec_t &prc){
    *const_cast<mpfr_prec_t*>(Number::defaultPrc) = prc / std::log10(2) + 1;
    ten = new Number();two = new Number();one = new Number();pi = new Number();e = new Number();half = new Number(); Number::negOne = new Number();
    mpfr_init2(*const_cast<mpfr_t*>(&Number::two->value), *Number::defaultPrc);
    mpfr_set_d(*const_cast<mpfr_t*>(&Number::two->value), 2.0, MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::ten->value), *Number::defaultPrc);
    mpfr_set_d(*const_cast<mpfr_t*>(&Number::ten->value), 10.0, MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::one->value), *Number::defaultPrc);
    mpfr_set_d(*const_cast<mpfr_t*>(&Number::one->value), 1.0, MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::half->value), *Number::defaultPrc);
    mpfr_set_d(*const_cast<mpfr_t*>(&Number::half->value), 0.5, MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::negOne->value), *Number::defaultPrc);
    mpfr_set_d(*const_cast<mpfr_t*>(&Number::negOne->value), -1.0, MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::pi->value), *Number::defaultPrc);
    mpfr_const_pi(*const_cast<mpfr_t*>(&Number::pi->value), MPFR_RNDN);
    mpfr_init2(*const_cast<mpfr_t*>(&Number::e->value), *Number::defaultPrc);
    mpfr_exp(*const_cast<mpfr_t*>(&Number::e->value), Number::one->value, MPFR_RNDN);
}

void Number::deInit(){
    Number::e->~Number();
    Number::pi->~Number();
    Number::one->~Number();
    Number::ten->~Number();
    Number::two->~Number();
    Number::negOne->~Number();
    Number::half->~Number();
    mpfr_free_cache();
}

Number::Number(){
    mpfr_init2(value, *Number::defaultPrc);
    mpfr_set_zero(value, 1);
}

Number::~Number(){
    mpfr_clear(value);
}

Number::Number(const Number &other){
    this->value->_mpfr_d = other.value->_mpfr_d;
    this->value->_mpfr_exp = other.value->_mpfr_exp;
    this->value->_mpfr_prec = other.value->_mpfr_prec;
    this->value->_mpfr_sign = other.value->_mpfr_sign;
}

void Number::operator = (const Number &other){
    mpfr_set(value, other.value, MPFR_RNDN);
}

void Number::operator = (const long double &num){
    mpfr_set_d(value, num, MPFR_RNDN);
}

void Number::operator = (const mpfr_t &other){
    mpfr_set(value, other, MPFR_RNDN);
}

Number Number::operator + (const Number &other) const{
    Number num;
    mpfr_add(num.value, value, other.value, MPFR_RNDN);
    return num;
}

Number Number::operator - ()const{
    Number num;
    mpfr_neg(num.value, num.value, MPFR_RNDN);
    return num;
}

Number Number::operator - (const Number &other) const{
    Number num;
    mpfr_sub(num.value, value, other.value, MPFR_RNDN);
    return num;
}

Number Number::operator * (const Number &other)const{
    Number num;
    if(!mpfr_equal_p(value, other.value)) mpfr_mul(num.value, value, other.value, MPFR_RNDN);
    else mpfr_sqr(num.value, value, MPFR_RNDN);
    return num;
}

Number Number::operator / (const Number &other) const{
    Number num;
    mpfr_div(num.value, value, other.value, MPFR_RNDN);
    return num;
}

Number Number::operator ^ (const Number &other) const{
    Number num;
    mpfr_pow(num.value, value, other.value, MPFR_RNDN);
    return num;
}

void Number::operator += (const Number &other){
    mpfr_add(value, value, other.value, MPFR_RNDN);
}

void Number::operator -= (const Number &other){
    mpfr_sub(value, value, other.value, MPFR_RNDN);
}

void Number::operator *= (const Number &other){
    if(!mpfr_equal_p(value, other.value)) mpfr_mul(value, value, other.value, MPFR_RNDN);
    else mpfr_sqr(value, value, MPFR_RNDN);
}

void Number::operator /= (const Number &other){
    mpfr_div(value, value, other.value, MPFR_RNDN);
}

void Number::operator ^= (const Number &other){
    mpfr_pow(value, value, other.value, MPFR_RNDN);
}

void Number::logSelf(const Number& base){
    if (mpfr_equal_p(base.value, Number::e->value)) mpfr_log(value, value, MPFR_RNDN);
    else if (mpfr_equal_p(base.value, Number::ten->value)){
        mpfr_log10(value, value, MPFR_RNDN);
    }
    else if (mpfr_equal_p(base.value, Number::two->value)){
        mpfr_log2(value, value, MPFR_RNDN);
    }
    else{
        Number num;
        num = base;
        mpfr_log(value, value, MPFR_RNDN);
        mpfr_log(num.value, num.value, MPFR_RNDN);
        mpfr_div(value, value, num.value, MPFR_RNDN);
    }
}

Number Number::log(const Number& base) const{
    Number num;
    mpfr_set(num.value, value, MPFR_RNDN);
    num.logSelf(base);
    return num;
}

Number Number::abs() const{
    Number num;
    mpfr_set(num.value, value, MPFR_RNDN);
    num.absSelf();
    return num;
}

void Number::absSelf(){
    mpfr_abs(value, value, MPFR_RNDN);
}

Number Number::cos() const{
    Number num;
    mpfr_set(num.value, value, MPFR_RNDN);
    num.cosSelf();
    return num;
}

void Number::cosSelf(){
    mpfr_cos(value, value, MPFR_RNDN);
}

Number Number::sin() const{
    Number num;
    mpfr_set(num.value, value, MPFR_RNDN);
    num.sinSelf();
    return num;
}

void Number::sinSelf(){
    mpfr_sin(value, value, MPFR_RNDN);
}

