#include "lexer.hpp"
#include "tokens/token.hpp"

Lexer::Lexer(const std::string p) : path(p), content(ReadEntireFile())
{
}

std::string Lexer::ReadEntireFile()
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[Lexer] ERROR: Couldn't open file -> " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string tmp;
    std::stringstream ss;
    while (std::getline(file, tmp))
        ss << tmp << std::endl;

    return ss.str();
}

bool Lexer::IsWhitespace(const char &character) const
{
    return static_cast<bool>(std::iswspace(static_cast<wchar_t>(character)));
}

bool Lexer::IsAlpha(const char &character) const
{
    return static_cast<bool>(std::isalpha(static_cast<wchar_t>(character)));
}

bool Lexer::IsAlphaNum(const char &character) const
{
    return static_cast<bool>(std::isalnum(static_cast<wchar_t>(character)));
}

bool Lexer::IsNum(const char &character) const
{
    return static_cast<bool>(std::isdigit(static_cast<wchar_t>(character)));
}

void Lexer::SkipWhitespace()
{
    const std::size_t length = content.size();
    while (index < length && IsWhitespace(content.at(index)))
    {
        if (content.at(index++) == '\n')
        {
            ++line;
            lineStartIndex = index;
        }
    }
}

std::unique_ptr<Token> Lexer::GetAlphaNumToken()
{
    std::stringstream ss;
    const size_t length = content.size();
    const size_t column = GetCurrentColumnNumber();

    while (index < length && IsAlphaNum(content[index]))
        ss << content[index++];

    const std::string lexeme = ss.str();

    TokenPosition position = (TokenPosition){.line = line, .column = column};
    TokenType type;
    if (!GetTypeByLexeme(lexeme, type))
        return std::make_unique<Token>(Token(lexeme, TokenType::IDENTIFIER, position));

    return std::make_unique<Token>(Token(lexeme, type, (TokenPosition){.line = line, .column = column}));
}

std::unique_ptr<Token> Lexer::GetNumToken()
{
    std::stringstream ss;
    const size_t length = content.size();
    const size_t column = GetCurrentColumnNumber();

    while (index < length && IsNum(content[index]))
        ss << content[index++];

    const std::string lexeme = ss.str();
    TokenPosition position = (TokenPosition){.line = line, .column = column};
    return std::make_unique<Token>(Token(lexeme, TokenType::NUM_LITERAL, (TokenPosition){.line = line, .column = GetCurrentColumnNumber()}));
}

std::unique_ptr<Token> Lexer::GetToken()
{
    const size_t length = content.size();
    const size_t column = GetCurrentColumnNumber();

    TokenType lastValidTokenType;
    bool hasFoundValidToken = false;
    size_t lastValidTokenEndIndex = 0;

    while (index < length && !IsWhitespace(content[index]) && !IsAlphaNum(content[index]))
    {
        ++index;
        if (!GetTypeByLexeme(content.substr(lastStopIndex, index - lastStopIndex), lastValidTokenType))
        {
            if (!hasFoundValidToken)
                continue;

            const std::string lexeme = content.substr(lastStopIndex, lastValidTokenEndIndex - lastStopIndex);
            return std::make_unique<Token>(Token(lexeme, lastValidTokenType, (TokenPosition){.line = line, .column = column}));
        }

        hasFoundValidToken = true;
        lastValidTokenEndIndex = index;
    }

    if (hasFoundValidToken)
    {
        const std::string lexeme = content.substr(lastStopIndex, lastValidTokenEndIndex - lastStopIndex);
        return std::make_unique<Token>(Token(lexeme, lastValidTokenType, (TokenPosition){.line = line, .column = column}));
    }

    return nullptr;
}

size_t Lexer::GetCurrentColumnNumber() const
{
    return index - lineStartIndex + 1;
}

std::unique_ptr<Token> Lexer::GetNextToken()
{
    const char &c = content[index];

    if (IsAlpha(c))
        return GetAlphaNumToken();

    if (IsNum(c))
        return GetNumToken();

    return GetToken();
}

bool Lexer::GetTypeByLexeme(std::string_view lexeme, TokenType &type)
{
    const auto iterator = TOKENS.find(lexeme);
    if (iterator == TOKENS.end())
        return false;

    type = iterator->second;
    return true;
}

bool Lexer::Tokenize()
{
    std::cout << "Tokenizing -> " << path << std::endl;
    std::cout << "Content:" << std::endl;
    std::cout << content << std::endl;

    const std::size_t length = content.size();

    SkipWhitespace();
    size_t errors = 0;
    while (index < length)
    {
        lastStopIndex = index;

        std::unique_ptr<Token> token = GetNextToken();
        if (token == nullptr)
        {
            std::cout << "ERROR: Unknown token at line " << line << " column " << lastStopIndex - lineStartIndex + 1 << " -> " << content.substr(lastStopIndex, index - lastStopIndex) << std::endl;
            ++errors;
        }
        else
        {
            std::cout << *token << std::endl;
            tokens.push_back(std::move(token));
        }
        SkipWhitespace();
    }

    return errors == 0;
}

std::vector<std::unique_ptr<const Token>> &Lexer::GetTokens()
{
    return tokens;
}