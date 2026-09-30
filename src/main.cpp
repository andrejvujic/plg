#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "lexer/lexer.hpp"
#include "lexer/tokens/token.hpp"

#include "parser/parser.hpp"

#include "semantic_analysis/semantic_analyzer.hpp"

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
    p.Parse();
    std::cout << "[Parser] Success" << std::endl;

    auto &ast = p.GetAst();
    AstPrinter printer(std::cout);
    ast->Print(printer);

    SemanticAnalyzer s;
    s.AnalyzeProgram(ast);

    return EXIT_SUCCESS;
}
