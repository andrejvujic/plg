#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <string>
#include <iostream>

#include "lexer/tokens/token_type.hpp"
#include "shared/location.hpp"

class Token
{
protected:
    Location location;
    TokenType type;
    std::string name;

private:
    std::string lexeme;

public:
    Token();

    Token(std::string lexeme, TokenType type, Location location);

    const std::string &GetLexeme() const;

    Location GetLocation() const;

    TokenType GetType() const;
    bool Matches(TokenType type) const;
    bool MatchesAny(const std::initializer_list<TokenType> &types) const;

    friend std::ostream &operator<<(std::ostream &out, const Token &t);
};

#endif
