#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Function.hpp"
#include "Line.hpp"




int main() {
    std::fstream input("test.txt", std::ios::in);
    std::vector<Function> functions;
    for(std::string lineStr; std::getline(input,lineStr);){
        Line line(lineStr, Tokenizer(lineStr).tokenize());
        if(line.getLineType() == LineType::funDef){
            functions.push_back(Function(ParseTree(Tokenizer(line.getFunDef()).tokenize()).Parse(), line.getFunName()));
        }
        else if(line.getLineType() == LineType::showFunVal){
            for (auto fun : functions) {
                if(fun.getName() == line.getFunName()){
                    
                    std::cout << line.getFunName() << "(" << line.getFunVal() << ") = " << fun(line.getFunVal()) << std::endl;
                }
            }
        }
    }
    
    return 0;
    
    
}
