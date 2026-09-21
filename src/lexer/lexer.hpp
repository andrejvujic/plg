#ifndef LEXER_HPP_DEFINED
#define LEXER_HPP_DEFINED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

#include "tokens/token.hpp"
#include "tokens/token_type.hpp"

class Lexer
{
private:
    const std::string path;
    const std::string content;
    std::size_t index = 0, lastStopIndex = 0;
    std::size_t line = 1, lineStartIndex = 0;
    std::string ReadEntireFile();
    std::vector<std::unique_ptr<const Token>> tokens;

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
    Lexer(std::string path);

    bool Tokenize();
    std::vector<std::unique_ptr<const Token>> &GetTokens();
};

static const std::map<std::string_view, TokenType> TOKENS{
    {"if", TokenType::KW_IF},
    {"return", TokenType::KW_RETURN},
    {"function", TokenType::KW_FUNCTION},
    {"int", TokenType::KW_TYPE},
    {"+", TokenType::AR_OPERATOR},
    {"-", TokenType::AR_OPERATOR},
    {"*", TokenType::AR_OPERATOR},
    {"!=", TokenType::REL_OPERATOR},
    {">", TokenType::REL_OPERATOR},
    {"<", TokenType::REL_OPERATOR},
    {"==", TokenType::REL_OPERATOR},
    {"=", TokenType::ASSIGNMENT},
    {"->", TokenType::ARROW},
    {"{", TokenType::O_CURLY},
    {"}", TokenType::C_CURLY},
    {"(", TokenType::O_PAREN},
    {")", TokenType::C_PAREN},
    {":", TokenType::COLON},
    {";", TokenType::SEMICOLON},
};

#endif