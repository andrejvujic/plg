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
    KEYWORD_TYPE = 8,
    ARROW = 16,
    ID = 32,
    NUM_LITERAL = 64,
    O_CURLY = 128,
    C_CURLY = 256,
    O_PAREN = 512,
    C_PAREN = 1024,
    COLON = 2048,
    SEMICOLON = 4096,
    ASSIGNMENT = 8192,
    ARITHMETIC_OPERATOR = 16384,
    RELATIONAL_OPERATOR = 32768,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
