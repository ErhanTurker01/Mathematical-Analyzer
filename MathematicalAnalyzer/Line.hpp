//
//  Line.hpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 2/27/24.
//

#include <string>
#include "Tokenizer.hpp"
#include "Number.hpp"
#ifndef Line_hpp
#define Line_hpp

enum class LineType {
    funDef,
    varDef,
    showVal,
    showFun,
    showFunVal
};

class Line {
    std::string src;
    LineType type;
    std::vector<Token> tokens;
public:
    Line(std::string line, std::vector<Token> tokens);
    Line(){};
    std::string getFunDef();
    std::string getFunName();
    std::string getVarVal();
    std::string getVarName();
    Number getFunVal();
    LineType getLineType();
};

#endif /* Line_hpp */
