#include "token_type.hpp"

// https://stackoverflow.com/a/3342891/13646430
std::ostream &operator<<(std::ostream &out, const TokenType &value)
{
    static const std::map<TokenType, std::string_view> strings = []
    {
        std::map<TokenType, std::string_view> result;

#define INSERT_ELEMENT(p) result.emplace(p, #p);
        INSERT_ELEMENT(KEYWORD_FUNCTION);
        INSERT_ELEMENT(KEYWORD_RETURN);
        INSERT_ELEMENT(KEYWORD_IF);
        INSERT_ELEMENT(KEYWORD_TYPE);
        INSERT_ELEMENT(ARROW);
        INSERT_ELEMENT(ID);
        INSERT_ELEMENT(NUM_LITERAL);
        INSERT_ELEMENT(O_CURLY);
        INSERT_ELEMENT(C_CURLY);
        INSERT_ELEMENT(O_PAREN);
        INSERT_ELEMENT(C_PAREN);
        INSERT_ELEMENT(COLON);
        INSERT_ELEMENT(SEMICOLON);
        INSERT_ELEMENT(ASSIGNMENT);
        INSERT_ELEMENT(ARITHMETIC_OPERATOR);
        INSERT_ELEMENT(RELATIONAL_OPERATOR);
#undef INSERT_ELEMENT

        return result;
    }();

    const auto iterator = strings.find(value);

    if (iterator != strings.end())
        return out << iterator->second;

    return out << "UNKNOWN_TOKEN";
}