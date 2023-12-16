#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Function.hpp"
#include "lineIdentifier.hpp"




int main() {
    lineIdentifier iden("test.txt");
    std::vector<Line> lines = iden.identifyLines();
    for(auto line: lines){
        std::cout << line.str << " " << (int)line.type << '\n';
    }
    
    
    return 0;
}
