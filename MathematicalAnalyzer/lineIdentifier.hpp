//
//  lineIdentifier.hpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 12/15/23.
//
#pragma once


#include <string>
#include "Tokenizer.hpp"

#ifndef lineIdentifier_hpp
#define lineIdentifier_hpp
enum class LineType{
    funDef,
    varDef,
    progFun
};

typedef struct{
    std::string str;
    LineType type;
}Line;

class lineIdentifier {
    std::string fileName;
    
private:
    LineType getLineType(std::string str);
public:
    std::vector<Line> identifyLines();
    lineIdentifier(std::string fileName):fileName(fileName){}
};

#endif /* lineIdentifier_hpp */
