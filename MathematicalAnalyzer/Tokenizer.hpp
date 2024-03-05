#pragma once


#include <string>
#include <optional>
#include <vector>

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

enum class TokenType{
    var,
    pl,
    pr,
    num,
    opr,
    fun,
    der,
    eq,
    other
};

enum class OprType{
    add,
    sub,
    mult,
    div,
    pow
};

enum class FunType{
    sin,
    cos,
    ln,
    abs
};


struct Token{
    TokenType type;
    std::optional<std::string> value;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
};

class Tokenizer{
    const std::string src;
    size_t index;
    
private:
    std::optional<char> peek(unsigned int ahead = 0) const;
    char consume(){return src.at(index++);}
    
public:
    Tokenizer(std::string src):src(src),index(0){}
    std::vector<Token> tokenize();
};






#endif /* tokenizer_hpp */
