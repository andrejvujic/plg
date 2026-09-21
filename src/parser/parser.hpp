#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>

#include "../lexer/tokens/token.hpp"

typedef const std::vector<std::unique_ptr<const Token>> Tokens;

class Parser
{
private:
    Tokens &tokens;
    size_t index = 0;

public:
    Parser(Tokens &t);

    bool Parse();
};

#endif
