#include "lexer/tokens/token.hpp"

const std::string &Token::GetLexeme() const
{
    return lexeme;
}

bool Token::LexemeEquals(std::string_view lexeme) const
{
    return this->lexeme == lexeme;
}

bool Token::LexemeEqualsAny(std::initializer_list<std::string_view> lexemes) const
{
    return std::any_of(
        lexemes.begin(),
        lexemes.end(),
        [this](auto lexeme)
        {
            return this->lexeme == lexeme;
        });
}

SourceLocation Token::GetLocation() const
{
    return location;
}

TokenType Token::GetType() const
{
    return type;
}

bool Token::Matches(TokenType type) const
{
    return static_cast<bool>(this->type & type);
}

bool Token::MatchesAny(const std::initializer_list<TokenType> &types) const
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
