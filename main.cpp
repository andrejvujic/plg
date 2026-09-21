#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "src/lexer/lexer.hpp"
#include "src/lexer/tokens/token.hpp"

#include "src/parser/parser.hpp"

int main()
{
    const std::string path = "samples/sample_2.plg";

    std::cout << "Tokenizing -> " << path << std::endl;
    Lexer l(path);
    if (!l.Tokenize())
        return EXIT_FAILURE;
    std::cout << "Tokenized successfully." << std::endl;

    std::cout << "Parsing -> " << path << std::endl;
    Parser p(l.GetTokens());
    if (!p.Parse())
        return EXIT_FAILURE;
    std::cout << "Parsed successfully." << std::endl;

    return EXIT_SUCCESS;
}