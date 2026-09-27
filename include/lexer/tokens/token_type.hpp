#ifndef TOKEN_TYPE_HPP_INCLUDED
#define TOKEN_TYPE_HPP_INCLUDED

#include <map>
#include <string_view>
#include <iostream>

enum TokenType
{
    KEYWORD_FUNCTION = 1,
    KEYWORD_RETURN = 2,
    KEYWORD_IF = 4,
    KEYWORD_ELSE = 8,
    KEYWORD_TYPE = 16,
    KEYWORD_FOR = 32,
    ID = 64,
    NUM_LITERAL = 128,
    O_CURLY = 256,
    C_CURLY = 512,
    O_PAREN = 1024,
    C_PAREN = 2048,
    COLON = 4096,
    SEMICOLON = 8192,
    COMMA = 16384,
    ASSIGNMENT = 32768,
    ARITHMETIC_OPERATOR = 65536,
    RELATIONAL_OPERATOR = 131072,
    ARROW = 262144,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
