#ifndef TOKEN_TYPE_HPP_INCLUDED
#define TOKEN_TYPE_HPP_INCLUDED

#include <map>
#include <string_view>
#include <iostream>

enum TokenType
{
    KEYWORD_FUNCTION = 1 << 0,
    KEYWORD_RETURN = 1 << 1,
    KEYWORD_IF = 1 << 2,
    KEYWORD_ELSE = 1 << 3,
    KEYWORD_TYPE = 1 << 4,
    KEYWORD_FOR = 1 << 5,
    ID = 1 << 6,
    NUMBER = 1 << 7,
    O_CURLY = 1 << 8,
    C_CURLY = 1 << 9,
    O_PAREN = 1 << 10,
    C_PAREN = 1 << 11,
    COLON = 1 << 12,
    SEMICOLON = 1 << 13,
    COMMA = 1 << 14,
    ASSIGNMENT = 1 << 15,
    ARITHMETIC_OPERATOR = 1 << 16,
    RELATIONAL_OPERATOR = 1 << 17,
    ARROW = 1 << 18,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
