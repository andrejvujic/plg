#include "token.hpp"

Token::Token() {}

Token::Token(std::string l, TokenType t, TokenPosition p) : lexeme(l), type(t), position(p)
{
}

const std::string &Token::GetLexeme() const
{
    return lexeme;
}

size_t Token::GetLineNumber() const
{
    return position.line;
}

size_t Token::GetColumnNumber() const
{
    return position.column;
}

TokenType Token::GetType() const
{
    return type;
}

bool Token::Matches(TokenType type) const
{
    return static_cast<bool>(this->type & type);
}

bool Token::MatchesAny(std::initializer_list<TokenType> types) const
{
    int targetTypes = 0;
    for (auto type : types)
        targetTypes |= type;

    return static_cast<bool>(this->type & targetTypes);
}

std::ostream &operator<<(std::ostream &out, const Token &token)
{
    return out << token.type << " -> " << "\"" << token.lexeme << "\"";
}
