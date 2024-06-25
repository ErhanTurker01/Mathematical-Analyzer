#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Function.hpp"
#include "Line.hpp"
#include "Number.hpp"



int main() {
    Number::init();
    std::fstream input("test.txt", std::ios::in);
    {
        Parser parser;
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
                    parser.addFunction(line.getFunName(), parser.parse(Tokenizer(line.getFunDef()).tokenize()));
                    break;
                }
                case LineType::showFunVal:
                {
                    parser.showFunctionCalculation(line.getFunName(), line.getFunVal());
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
    }
    Number::deInit();
    return 0;
}
