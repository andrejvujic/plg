#include "lexer/tokens/token_type.hpp"

// https://stackoverflow.com/a/3342891/13646430
std::ostream &operator<<(std::ostream &out, const TokenType &value)
{
    static const std::map<TokenType, std::string_view> strings = []
    {
        std::map<TokenType, std::string_view> result;

#define INSERT_ELEMENT(p) result.emplace(p, #p);
        INSERT_ELEMENT(TokenType::KeywordFunction);
        INSERT_ELEMENT(TokenType::KeywordReturn);
        INSERT_ELEMENT(TokenType::KeywordIf);
        INSERT_ELEMENT(TokenType::KeywordElse);
        INSERT_ELEMENT(TokenType::KeywordType);
        INSERT_ELEMENT(TokenType::KeywordFor);
        INSERT_ELEMENT(TokenType::KeywordTrue);
        INSERT_ELEMENT(TokenType::KeywordWhile)
        INSERT_ELEMENT(TokenType::KeywordFalse);
        INSERT_ELEMENT(TokenType::Id);
        INSERT_ELEMENT(TokenType::Number);
        INSERT_ELEMENT(TokenType::OCurly);
        INSERT_ELEMENT(TokenType::CCurly);
        INSERT_ELEMENT(TokenType::OParen);
        INSERT_ELEMENT(TokenType::CParen);
        INSERT_ELEMENT(TokenType::Colon);
        INSERT_ELEMENT(TokenType::Semicolon);
        INSERT_ELEMENT(TokenType::Comma);
        INSERT_ELEMENT(TokenType::Assignment);
        INSERT_ELEMENT(TokenType::ArithmeticOperator);
        INSERT_ELEMENT(TokenType::RelationalOperator);
        INSERT_ELEMENT(TokenType::LogicalOperator);
        INSERT_ELEMENT(TokenType::Arrow);
#undef INSERT_ELEMENT

        return result;
    }();

    const auto iterator = strings.find(value);

    if (iterator != strings.end())
        return out << iterator->second;

    return out << "UNKNOWN_TOKEN";
}
