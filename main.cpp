#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "src/lexer/lexer.hpp"
#include "src/lexer/tokens/token.hpp"

#include "src/parser/parser.hpp"

std::string ReadEntireFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[Lexer] ERROR: Couldn't open file -> " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string tmp;
    std::stringstream ss;
    while (std::getline(file, tmp))
        ss << tmp << std::endl;

    return ss.str();
}

int main()
{
    const std::string path = "samples/sample_2.plg";

    std::cout << "Tokenizing -> " << path << std::endl;

    Lexer l(ReadEntireFile(path));
    if (!l.Tokenize())
        return EXIT_FAILURE;
    std::cout << "[Lexer] Success" << std::endl;

    std::cout << "Parsing -> " << path << std::endl;
    Parser p(l.GetTokens());
    if (!p.Parse())
        return EXIT_FAILURE;
    std::cout << "[Parser] Success" << std::endl;

    return EXIT_SUCCESS;
}