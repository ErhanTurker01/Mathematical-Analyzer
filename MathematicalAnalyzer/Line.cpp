//
//  Line.cpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 2/27/24.
//

#include "Line.hpp"

Line::Line(std::string line, std::vector<Token> tokens):src(line),tokens(tokens){
    if (std::find(line.begin(), line.end(), '=') != line.end()) {
        if (tokens.size() >= 5
            && tokens[0].type == TokenType::other
            && tokens[1].type == TokenType::pl
            && tokens[2].type == TokenType::var
            && tokens[3].type == TokenType::pr
            && tokens[4].type == TokenType::eq)
            type = LineType::funDef;
        else if (tokens.size() >=2
                 && tokens[0].type == TokenType::other
                 && tokens[1].type == TokenType::eq)
            type = LineType::varDef;
        else throw "what";
    }
    else if(tokens.size() >= 4
            && tokens[0].type == TokenType::other
            && tokens[1].type == TokenType::pl
            && tokens[2].type == TokenType::var
            && tokens[3].type == TokenType::pr)
        type = LineType::showFun;
    else if(tokens.size() >= 4
            && tokens[0].type == TokenType::other
            && tokens[1].type == TokenType::pl
            && tokens[2].type == TokenType::num
            && tokens[3].type == TokenType::pr)
        type = LineType::showFunVal;
    else if(tokens.size() == 1 && tokens[0].type == TokenType::other)
        type = LineType::showVal;
    else throw "what";
}

LineType Line::getLineType(){
    return type;
}

std::string Line::getFunDef(){
    return src.substr(src.find('=')+1);
}

std::string Line::getFunName(){
    return tokens[0].value.value();
}
Number Line::getFunVal(){
    return Number::getFromString(tokens[2].value.value());
}

std::string Line::getVarName(){
    return tokens[0].value.value();
}

std::string Line::getVarVal(){
    return src.substr(src.find('=')+1);
}
