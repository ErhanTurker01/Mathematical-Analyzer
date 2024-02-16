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
    std::string contents;
        {
            std::stringstream contents_stream;
            std::fstream input("test.txt", std::ios::in);
            contents_stream << input.rdbuf();
            contents = contents_stream.str();
        }
    std::cout << contents << std::endl;
    Function fun(ParseTree(Tokenizer(contents).tokenize()).Parse());
    std::cout << fun(3.5) << '\n';
    return 0;
}
