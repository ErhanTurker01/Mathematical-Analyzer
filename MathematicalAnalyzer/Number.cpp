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
