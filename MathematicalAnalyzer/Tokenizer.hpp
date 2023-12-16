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
    fun
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
    cos
};




typedef struct{
    TokenType type;
    std::optional<std::string> value;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
}Token;

class Tokenizer{
    const std::string src;
    size_t index;
    
private:
    std::optional<char> peek(unsigned int ahead = 0) const;
    char consume(){return src.at(index++);}
    
public:
    Tokenizer(std::string src):src(std::move(src)),index(0){}
    std::vector<Token> tokenize();
};






#endif /* tokenizer_hpp */
