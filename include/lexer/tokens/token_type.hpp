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
    KEYWORD_WHILE = 1 << 6,
    KEYWORD_TRUE = 1 << 7,
    KEYWORD_FALSE = 1 << 8,
    ID = 1 << 9,
    NUMBER = 1 << 10,
    O_CURLY = 1 << 11,
    C_CURLY = 1 << 12,
    O_PAREN = 1 << 13,
    C_PAREN = 1 << 14,
    COLON = 1 << 15,
    SEMICOLON = 1 << 16,
    COMMA = 1 << 17,
    ASSIGNMENT = 1 << 18,
    ARITHMETIC_OPERATOR = 1 << 19,
    RELATIONAL_OPERATOR = 1 << 20,
    LOGICAL_OPERATOR = 1 << 21,
    ARROW = 1 << 22,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
