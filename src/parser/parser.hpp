#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>

#include "../lexer/tokens/token.hpp"

class Parser
{
private:
    const std::vector<std::unique_ptr<const Token>> &tokens;
    size_t index = 0;

    bool HasMoreTokens() const;
    bool NextMatches(TokenType type) const;
    bool NextMatchesAny(std::initializer_list<TokenType> types) const;

    bool ParseFunction();

    bool ParseComplexStatement();
    bool ParseStatement();
    bool ParseAssignmentStatement();
    bool ParseIfStatement();
    bool ParseReturnStatement();

    bool ParseExpression();

    bool Consume(TokenType type);
    bool ConsumeAny(std::initializer_list<TokenType> types);

public:
    Parser(const std::vector<std::unique_ptr<const Token>> &t);

    bool Parse();
};

#endif
