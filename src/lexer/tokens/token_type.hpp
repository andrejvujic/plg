#ifndef TOKEN_TYPE_HPP_INCLUDED
#define TOKEN_TYPE_HPP_INCLUDED

#include <map>
#include <string_view>
#include <iostream>

enum TokenType
{
    KW_FUNCTION,
    KW_RETURN,
    KW_IF,
    KW_TYPE,
    ARROW,
    IDENTIFIER,
    NUM_LITERAL,
    O_CURLY,
    C_CURLY,
    O_PAREN,
    C_PAREN,
    COLON,
    SEMICOLON,
    ASSIGNMENT,
    AR_OPERATOR,
    REL_OPERATOR,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif