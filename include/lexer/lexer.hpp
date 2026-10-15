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
    {"if", TokenType::KEYWORD_IF},
    {"else", TokenType::KEYWORD_ELSE},
    {"return", TokenType::KEYWORD_RETURN},
    {"function", TokenType::KEYWORD_FUNCTION},
    {"int", TokenType::KEYWORD_TYPE},
    {"uint", TokenType::KEYWORD_TYPE},
    {"bool", TokenType::KEYWORD_TYPE},
    {"void", TokenType::KEYWORD_TYPE},
    {"for", TokenType::KEYWORD_FOR},
    {"true", TokenType::KEYWORD_TRUE},
    {"false", TokenType::KEYWORD_FALSE},
    {"+", TokenType::ARITHMETIC_OPERATOR},
    {"-", TokenType::ARITHMETIC_OPERATOR},
    {"*", TokenType::ARITHMETIC_OPERATOR},
    {"/", TokenType::ARITHMETIC_OPERATOR},
    {"!=", TokenType::RELATIONAL_OPERATOR},
    {">", TokenType::RELATIONAL_OPERATOR},
    {">=", TokenType::RELATIONAL_OPERATOR},
    {"<", TokenType::RELATIONAL_OPERATOR},
    {"<=", TokenType::RELATIONAL_OPERATOR},
    {"==", TokenType::RELATIONAL_OPERATOR},
    {"&&", TokenType::LOGICAL_OPERATOR},
    {"||", TokenType::LOGICAL_OPERATOR},
    {"=", TokenType::ASSIGNMENT},
    {"->", TokenType::ARROW},
    {"{", TokenType::O_CURLY},
    {"}", TokenType::C_CURLY},
    {"(", TokenType::O_PAREN},
    {")", TokenType::C_PAREN},
    {":", TokenType::COLON},
    {";", TokenType::SEMICOLON},
    {",", TokenType::COMMA},
};

#endif
