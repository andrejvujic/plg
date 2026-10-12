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
    KEYWORD_TRUE = 1 << 6,
    KEYWORD_FALSE = 1 << 7,
    ID = 1 << 8,
    NUMBER = 1 << 9,
    O_CURLY = 1 << 10,
    C_CURLY = 1 << 11,
    O_PAREN = 1 << 12,
    C_PAREN = 1 << 13,
    COLON = 1 << 14,
    SEMICOLON = 1 << 15,
    COMMA = 1 << 16,
    ASSIGNMENT = 1 << 17,
    ARITHMETIC_OPERATOR = 1 << 18,
    RELATIONAL_OPERATOR = 1 << 19,
    ARROW = 1 << 20,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
