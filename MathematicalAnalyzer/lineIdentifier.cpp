//
//  lineIdentifier.cpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 12/15/23.
//
#include <fstream>
#include "lineIdentifier.hpp"

LineType lineIdentifier::getLineType(std::string str){
    size_t i;
    if((i = str.find('=')) != std::string::npos){
        if(std::string(str.begin(), str.begin()+i).find('(') != std::string::npos){
            return LineType::funDef;
        }
        return LineType::varDef;
    }
    else{
        return LineType::progFun;
    }
}

std::vector<Line> lineIdentifier::identifyLines(){
    std::vector<Line> lines;
    std::fstream file(fileName, std::ios::in);
    if (!file.is_open()) throw "Can't open file";
    for (std::string line; std::getline(file, line); lines.push_back({.str = line, .type = getLineType(line)}));
    file.close();
    return lines;
}

