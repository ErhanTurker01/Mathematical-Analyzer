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
    Number::ten->~Number();
    Number::two->~Number();
    Number::one->~Number();
    Number::pi->~Number();
    Number::e->~Number();
    Number::half->~Number();
    Number::negOne->~Number();
    mpfr_free_cache();

#ifdef NumberDebug
    std::cout << '\n' << all << std::endl;
#endif
}

Number Number::getFromString(std::string str){
    Number num;
    mpfr_set_str(num.value, &str[0], 10, MPFR_RNDN);
    return num;
}

Number::Number(){
#ifdef NumberDebug
    id = ++count;
    all++;
    std::cout << "'" << id << "' " << std::flush;
#endif
    mpfr_init2(value, *Number::defaultPrc);
    mpfr_set_zero(value, 1);
}

Number::~Number(){
#ifdef NumberDebug
    all--;
    std::cout << "-" << id << "- " << std::flush;
#endif
    mpfr_clear(value);
}

Number::Number(const Number &other){
#ifdef NumberDebug
    id = ++count;
    all++;
    std::cout << "'" << id << "' " << std::flush;
#endif
    mpfr_init2(value, *Number::defaultPrc);
    mpfr_set(value, other.value, MPFR_RNDN);
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

void Number::print(mpfr_prec_t decimal)const{
    mpfr_printf("%.*RNf",decimal, value);
}


void ComplexNumber::operator = (const ComplexNumber& other){
    mpfr_set(real.value, other.real.value, MPFR_RNDN);
    mpfr_set(imaginary.value, other.imaginary.value, MPFR_RNDN);
}
void ComplexNumber::operator = (const Number& otherReal){
    mpfr_set(real.value, otherReal.value, MPFR_RNDN);
}
void ComplexNumber::operator = (const mpfr_t &other){
    mpfr_set(real.value, other, MPFR_RNDN);
}
void ComplexNumber::operator = (const long double &other){
    mpfr_set_d(real.value, other, MPFR_RNDN);
}

ComplexNumber ComplexNumber::operator + (const ComplexNumber &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx += (other);
    return cmplx;
}

ComplexNumber ComplexNumber::operator + (const Number &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx += (other);
    return cmplx;
}

ComplexNumber ComplexNumber::operator - (const ComplexNumber &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx -= (other);
    return cmplx;
}

ComplexNumber ComplexNumber::operator - (const Number &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx -= (other);
    return cmplx;
}

ComplexNumber ComplexNumber::operator - ()const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx.real *= *Number::negOne;
    cmplx.imaginary = *Number::negOne;
    return cmplx;
}

ComplexNumber ComplexNumber::operator * (const ComplexNumber &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx *= (other);
    return cmplx;
}

ComplexNumber ComplexNumber::operator * (const Number &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx *= other;
    return cmplx;
}

ComplexNumber ComplexNumber::operator / (const ComplexNumber &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx /= other;
    return cmplx;
}

ComplexNumber ComplexNumber::operator / (const Number &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx /= other;
    return cmplx;
}

ComplexNumber ComplexNumber::operator ^ (const ComplexNumber &other)const{
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx ^= (other);
    return cmplx;
}

void ComplexNumber::operator += (const ComplexNumber &other){
    real += other.real;
    imaginary += other.imaginary;
}

void ComplexNumber::operator += (const Number &other){
    real += other;
}

void ComplexNumber::operator -= (const ComplexNumber &other){
    real -= other.real;
    imaginary -= other.imaginary;
}

void ComplexNumber::operator -= (const Number &other){
    real -= other;
}

void ComplexNumber::operator *= (const ComplexNumber &other){
    Number temp;
    temp = real;
    real *= other.real;
    Number temp2 = imaginary * other.imaginary;
    real -= temp2;
    imaginary *= other.real;
    Number temp3 = temp * other.imaginary;
    imaginary += temp3;
}

void ComplexNumber::operator *= (const Number &other){
    real *= other;
    imaginary *= other;
}

void ComplexNumber::operator /= (const ComplexNumber &other){
    ComplexNumber con; con = other.conjugate();
    Number len; len = other.abs();
    len ^= *Number::two;
    *this *= con;
    *this /= len;
}

void ComplexNumber::operator /= (const Number &other){
    real /= other;
    imaginary /= other;
}

void ComplexNumber::operator ^= (const ComplexNumber &other){
    Number h = abs();
    h.logSelf(*Number::e);
    Number modulus; modulus = *Number::e;
    Number farg = arg();
    Number mod1 = other.real * h;
    Number mod2 = other.imaginary * farg;
    Number modDif = mod1 - mod2;
    modulus ^= modDif;

    h *= other.imaginary;
    Number arg1 = other.real * farg;
    h += arg1;
    real = modulus;
    Number cos1 = h.cos();
    Number sin1 = h.sin();
    real *= cos1;
    imaginary = modulus;
    imaginary *= sin1;

}

ComplexNumber ComplexNumber::log(const Number& base)const{
    ComplexNumber cmplx;
    cmplx.real = real;
    cmplx.imaginary = imaginary;
    cmplx.logSelf(base);
    return cmplx;
}

void ComplexNumber::logSelf(const Number& base){
    Number mod, argument, ebase;
    mod = abs();
    argument = arg();
    ebase = *Number::e;
    ebase.logSelf(base);
    real = mod;
    real.logSelf(base);
    imaginary = mod;
    imaginary *= ebase;
}

Number ComplexNumber::abs() const{
    Number num;
    ComplexNumber cmplx;
    cmplx = *this;
    cmplx.absSelf();
    num = cmplx.real;
    return num;
}

void ComplexNumber::absSelf(){
    real ^= *Number::two;
    imaginary ^= *Number::two;
    real += imaginary;
    real ^= *Number::half;
    imaginary = 0;
}

Number ComplexNumber::arg() const{
    Number num;
    ComplexNumber cmplx;
    cmplx.real = real;
    cmplx.imaginary = imaginary;
    cmplx.argSelf();
    num = cmplx.real;
    return num;
}

void ComplexNumber::argSelf(){
    imaginary /= real;
    mpfr_atan(real.value, imaginary.value, MPFR_RNDN);
    imaginary = 0;
}

ComplexNumber ComplexNumber::conjugate() const{
    ComplexNumber cmplx;
    cmplx.real = real;
    cmplx.imaginary = imaginary;
    cmplx.conjugateSelf();
    return cmplx;
}

void ComplexNumber::conjugateSelf(){
    mpfr_neg(imaginary.value, imaginary.value, MPFR_RNDN);
}

void ComplexNumber::print(mpfr_prec_t decimal){
    if(!mpfr_zero_p(real.value)) real.print(decimal);
    if(!mpfr_zero_p(imaginary.value)){
        if(mpfr_get_flt(imaginary.value, MPFR_RNDN) < 0){
            Number temp = imaginary.abs();
            printf(" - ");
            temp.print(decimal);
            printf("i\n");
        }
        else{
            Number temp = imaginary.abs();
            printf(" + ");
            temp.print(decimal);
            printf("i\n");
        }
    }
}
