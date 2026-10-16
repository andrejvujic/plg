#ifndef LEXER_HPP_DEFINED
#define LEXER_HPP_DEFINED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <utility>

#include "lexer/tokens/token.hpp"
#include "lexer/tokens/token_type.hpp"

class Lexer
{
private:
    const std::string text;
    std::size_t index = 0, lastStopIndex = 0;
    std::size_t line = 1, lineStartIndex = 0;
    std::string ReadEntireFile();
    std::vector<std::unique_ptr<Token>> tokens;

    bool IsWhitespace(const char &c) const;
    bool IsAlpha(const char &c) const;
    bool IsAlphaNum(const char &c) const;
    bool IsNum(const char &c) const;

    void SkipWhitespace();

    std::unique_ptr<Token> GetAlphaNumToken();
    std::unique_ptr<Token> GetNumToken();
    std::unique_ptr<Token> GetToken();
    std::unique_ptr<Token> GetNextToken();

    bool GetTypeByLexeme(std::string_view lexeme, TokenType &type);

    size_t GetCurrentColumnNumber() const;

public:
    explicit Lexer(std::string &&text) : text(std::move(text)) {}

    bool Tokenize();
    std::vector<std::unique_ptr<Token>> &GetTokens();
};

static const std::map<std::string_view, TokenType> TOKENS{
    {"if", TokenType::KeywordIf},
    {"else", TokenType::KeywordElse},
    {"return", TokenType::KeywordReturn},
    {"function", TokenType::KeywordFunction},
    {"int", TokenType::KeywordType},
    {"uint", TokenType::KeywordType},
    {"bool", TokenType::KeywordType},
    {"void", TokenType::KeywordType},
    {"for", TokenType::KeywordFor},
    {"while", TokenType::KeywordWhile},
    {"true", TokenType::KeywordTrue},
    {"false", TokenType::KeywordFalse},
    {"+", TokenType::ArithmeticOperator},
    {"-", TokenType::ArithmeticOperator},
    {"*", TokenType::ArithmeticOperator},
    {"/", TokenType::ArithmeticOperator},
    {"!=", TokenType::RelationalOperator},
    {">", TokenType::RelationalOperator},
    {">=", TokenType::RelationalOperator},
    {"<", TokenType::RelationalOperator},
    {"<=", TokenType::RelationalOperator},
    {"==", TokenType::RelationalOperator},
    {"&&", TokenType::LogicalOperator},
    {"||", TokenType::LogicalOperator},
    {"=", TokenType::Assignment},
    {"->", TokenType::Arrow},
    {"{", TokenType::OCurly},
    {"}", TokenType::CCurly},
    {"(", TokenType::OParen},
    {")", TokenType::CParen},
    {":", TokenType::Colon},
    {";", TokenType::Semicolon},
    {",", TokenType::Comma},
};

#endif
