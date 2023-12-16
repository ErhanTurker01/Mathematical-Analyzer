#include "Tokenizer.hpp"


/*From text file read char one by one and makes them meaningfull tokens*/
std::vector<Token> Tokenizer::tokenize(){
    std::vector<Token> tokens;
    std::string buf;
    while (peek().has_value()) {
        /*This part handels all variables and functions*/
        if(isalpha(peek().value())){
            while(peek().has_value() && isalpha(peek().value())) {
                buf += consume();
                if (buf == "x") {
                    tokens.push_back({.type = TokenType::var});
                    buf.clear();
                }
                else if(buf == "sin"){
                    tokens.push_back({.type = TokenType::fun, .funType = FunType::sin});
                    buf.clear();
                }
                else if(buf == "cos"){
                    tokens.push_back({.type = TokenType::fun, .funType = FunType::cos});
                    buf.clear();
                }
            }
            if(buf.size()){
                throw "Not defined var or fun";
            }
        }
        /*This part handles numbers*/
        else if(isdigit(peek().value())){
            bool firstPoint = true;
            while(peek().has_value() && (peek().value() == '.' || isdigit(peek().value()))) {
                if(peek().value() == '.' && !firstPoint) return {};
                if(peek().value() == '.' && firstPoint) firstPoint = false;
                buf += consume();
            }
            tokens.push_back({.type = TokenType::num, .value = buf});
            buf.clear();
        }
        else if(isspace(peek().value())){
            consume();
        }
        else if(peek().value() == '('){
            tokens.push_back({.type = TokenType::pl});
            consume();
        }
        else if(peek().value() == ')'){
            tokens.push_back({.type = TokenType::pr});
            consume();
        }
        else if(peek().value() == '+'){
            tokens.push_back({.type = TokenType::opr, .oprType = OprType::add});
            consume();
        }
        else if(peek().value() == '-'){
            tokens.push_back({.type = TokenType::opr, .oprType = OprType::sub});
            consume();
        }
        else if(peek().value() == '*'){
            tokens.push_back({.type = TokenType::opr, .oprType = OprType::mult});
            consume();
        }
        else if(peek().value() == '/'){
            tokens.push_back({.type = TokenType::opr, .oprType = OprType::div});
            consume();
        }
        else if(peek().value() == '^'){
            tokens.push_back({.type = TokenType::opr, .oprType = OprType::pow});
            consume();
        }
    }
    return tokens;
}
/*Cheks current index is safe to look at if not returns a empty optinal char*/
std::optional<char> Tokenizer::peek(unsigned int ahead) const{
    if(index + ahead >= src.length()) return {};
    else return src.at(index+ahead);
}
