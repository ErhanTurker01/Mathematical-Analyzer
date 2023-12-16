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
    Tokenizer tokenizer(contents);
    auto tokens = tokenizer.tokenize();
    ParseTree parser(tokens);
    auto start = parser.Parse();
    Function fun(start);
    fun(1);
    return 0;
}
