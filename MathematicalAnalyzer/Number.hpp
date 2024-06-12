//
//  Number.hpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 4/23/24.
//

#include <mpfr.h>

#ifndef Number_hpp
#define Number_hpp

struct Number{
    static const inline mpfr_prec_t* defaultPrc = new mpfr_prec_t;
    static const inline Number *e, *two, *ten, *pi, *one, *half, *negOne;
    static void init(const mpfr_prec_t &prc = 20);
    static void deInit();
    
    mpfr_t value;
    Number();
    Number(const Number &other);
    ~Number();
    void operator = (const Number &other);
    void operator = (const mpfr_t &other);
    void operator = (const long double &num);
    Number operator + (const Number &other)const;
    Number operator - (const Number &other)const;
    Number operator - ()const;
    Number operator * (const Number &other)const;
    Number operator / (const Number &other)const;
    Number operator ^ (const Number &other)const;
    void operator += (const Number &other);
    void operator -= (const Number &other);
    void operator *= (const Number &other);
    void operator /= (const Number &other);
    void operator ^= (const Number &other);
    Number log(const Number& base) const;
    void logSelf(const Number& base);
    Number abs() const;
    void absSelf();
    Number cos() const;
    void cosSelf();
    Number sin() const;
    void sinSelf();
    void print(mpfr_prec_t decimal);
};

struct ComplexNumber{
    Number real, imaginary;
    
    void operator = (const ComplexNumber& other);
    void operator = (const Number& otherReal);
    void operator = (const mpfr_t &other);
    void operator = (const long double &other);
    ComplexNumber operator + (const ComplexNumber &other)const;
    ComplexNumber operator - (const ComplexNumber &other)const;
    ComplexNumber operator + (const Number &other)const;
    ComplexNumber operator - (const Number &other)const;
    ComplexNumber operator - ()const;
    ComplexNumber operator * (const ComplexNumber &other)const;
    ComplexNumber operator / (const ComplexNumber &other)const;
    ComplexNumber operator ^ (const ComplexNumber &other)const;
    ComplexNumber operator * (const Number &other)const;
    ComplexNumber operator / (const Number &other)const;
    void operator += (const ComplexNumber &other);
    void operator -= (const ComplexNumber &other);
    void operator *= (const ComplexNumber &other);
    void operator /= (const ComplexNumber &other);
    void operator ^= (const ComplexNumber &other);
    void operator += (const Number &other);
    void operator -= (const Number &other);
    void operator *= (const Number &other);
    void operator /= (const Number &other);
    ComplexNumber log(const Number& base) const;
    void logSelf(const Number& base);
    Number abs() const;
    void absSelf();
    Number arg() const;
    void argSelf();
    ComplexNumber conjugate() const;
    void conjugateSelf();
    void print(mpfr_prec_t decimal);
};

#endif /* Number_hpp */
