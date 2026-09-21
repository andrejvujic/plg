#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "src/lexer/lexer.hpp"
#include "src/lexer/tokens/token.hpp"

#include "src/parser/parser.hpp"

int main()
{
    const std::string path = "samples/sample_1.plg";
    Lexer l(path);
    if (!l.Tokenize())
        return EXIT_FAILURE;

    Parser p(l.GetTokens());
    if (!p.Parse())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}