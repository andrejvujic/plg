#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <string>
#include <iostream>

#include "token_type.hpp"

typedef struct
{
    std::size_t line, column;
} TokenPosition;

class Token
{
protected:
    TokenPosition position;
    TokenType type;
    std::string name;

private:
    std::string lexeme;

public:
    Token();

    Token(std::string lexeme, TokenType t, TokenPosition p);

    const std::string &GetLexeme() const;

    size_t GetLineNumber() const;
    size_t GetColumnNumber() const;

    TokenType GetType() const;
    bool Matches(TokenType type) const;
    bool MatchesAny(const std::initializer_list<TokenType> &types) const;

    friend std::ostream &operator<<(std::ostream &out, const Token &t);
};

#endif
