#ifndef TOKEN_TYPE_HPP_INCLUDED
#define TOKEN_TYPE_HPP_INCLUDED

#include <map>
#include <string_view>
#include <iostream>

enum TokenType
{
    KeywordFunction = 1 << 0,
    KeywordReturn = 1 << 1,
    KeywordIf = 1 << 2,
    KeywordElse = 1 << 3,
    KeywordType = 1 << 4,
    KeywordFor = 1 << 5,
    KeywordWhile = 1 << 6,
    KeywordTrue = 1 << 7,
    KeywordFalse = 1 << 8,
    Id = 1 << 9,
    Number = 1 << 10,
    OCurly = 1 << 11,
    CCurly = 1 << 12,
    OParen = 1 << 13,
    CParen = 1 << 14,
    Colon = 1 << 15,
    Semicolon = 1 << 16,
    Comma = 1 << 17,
    Assignment = 1 << 18,
    ArithmeticOperator = 1 << 19,
    RelationalOperator = 1 << 20,
    LogicalOperator = 1 << 21,
    Arrow = 1 << 22,
};

std::ostream &operator<<(std::ostream &out, const TokenType &value);
#endif
