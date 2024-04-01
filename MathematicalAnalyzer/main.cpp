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
    Parser parser;
    std::vector<Function> functions;
    for(std::string lineStr; std::getline(input,lineStr);){
        std::vector<Token> tokens = Tokenizer(lineStr).tokenize();
        Line line(lineStr, Tokenizer(lineStr).tokenize());
        switch (line.getLineType()) {
            case LineType::varDef:
            {
                parser.addVariable(line.getVarName(), parser.parse(Tokenizer(line.getVarVal()).tokenize()));
                break;
            }
            case LineType::funDef:
            {
                Node* fun = parser.parse(Tokenizer(line.getFunDef()).tokenize());
                parser.addFunction(line.getFunName(), fun);
                functions.push_back(Function(fun, line.getFunName()));
                break;
            }
            case LineType::showFunVal:
            {
                for (Function fun : functions) if(fun.getName() == line.getFunName()) std::cout << line.getFunName() << "(" << line.getFunVal() << ") = " << fun(line.getFunVal()) << std::endl;
                break;
            }
            case LineType::showVal:
            {
                break;
            }
            case LineType::showFun:
            {
                break;
            }
        }/* End of switch */
    }/* End of lines */
    
    return 0;
    
    
}
