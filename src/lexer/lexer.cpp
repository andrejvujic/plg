#include "lexer/lexer.hpp"
#include "lexer/tokens/token.hpp"

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
    const std::size_t length = text.size();
    while (index < length && IsWhitespace(text.at(index)))
    {
        if (text.at(index++) == '\n')
        {
            ++line;
            lineStartIndex = index;
        }
    }
}

std::unique_ptr<Token> Lexer::GetAlphaNumToken()
{
    std::stringstream ss;
    const size_t length = text.size();
    const size_t column = GetCurrentColumnNumber();

    while (index < length && IsAlphaNum(text.at(index)))
        ss << text[index++];

    const std::string lexeme = ss.str();

    const SourceLocation location{.line = line, .column = column};
    TokenType type;
    if (!GetTypeByLexeme(lexeme, type))
        return std::make_unique<Token>(Token(lexeme, TokenType::ID, location));

    return std::make_unique<Token>(Token(lexeme, type, location));
}

std::unique_ptr<Token> Lexer::GetNumToken()
{
    std::stringstream ss;
    const size_t length = text.size();
    const size_t column = GetCurrentColumnNumber();

    while (index < length && IsNum(text.at(index)))
        ss << text[index++];

    if (index < length && text.at(index) == 'u')
        ss << text[index++];

    const std::string lexeme = ss.str();
    const SourceLocation location{.line = line, .column = column};

    return std::make_unique<Token>(Token(lexeme, TokenType::NUMBER, location));
}

std::unique_ptr<Token> Lexer::GetToken()
{
    const size_t length = text.size();
    const size_t column = GetCurrentColumnNumber();

    TokenType lastValidTokenType;
    bool hasFoundValidToken = false;
    size_t lastValidTokenEndIndex = 0;

    while (index < length && !IsWhitespace(text[index]) && !IsAlphaNum(text[index]))
    {
        ++index;
        if (!GetTypeByLexeme(text.substr(lastStopIndex, index - lastStopIndex), lastValidTokenType))
        {
            if (!hasFoundValidToken)
                continue;

            index = lastValidTokenEndIndex;
            const std::string lexeme = text.substr(lastStopIndex, lastValidTokenEndIndex - lastStopIndex);
            const SourceLocation location{.line = line, .column = column};
            return std::make_unique<Token>(Token(lexeme, lastValidTokenType, location));
        }

        hasFoundValidToken = true;
        lastValidTokenEndIndex = index;
    }

    if (hasFoundValidToken)
    {
        index = lastValidTokenEndIndex;
        const std::string lexeme = text.substr(lastStopIndex, lastValidTokenEndIndex - lastStopIndex);
        const SourceLocation location = {.line = line, .column = column};
        return std::make_unique<Token>(Token(lexeme, lastValidTokenType, location));
    }

    return nullptr;
}

size_t Lexer::GetCurrentColumnNumber() const
{
    return index - lineStartIndex + 1;
}

std::unique_ptr<Token> Lexer::GetNextToken()
{
    if (IsAlpha(text.at(index)))
        return GetAlphaNumToken();

    if (IsNum(text.at(index)))
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
    // std::cout << "text:" << std::endl;
    // std::cout << text << std::endl;

    const std::size_t length = text.size();

    SkipWhitespace();
    size_t errors = 0;
    while (index < length)
    {
        lastStopIndex = index;

        std::unique_ptr<Token> token = GetNextToken();
        if (token == nullptr)
        {
            std::cerr << "ERROR: line " << line << " column " << lastStopIndex - lineStartIndex + 1 << ": Unknown token -> " << text.substr(lastStopIndex, index - lastStopIndex) << std::endl;
            ++errors;
        }
        else
        {
            std::cout << *token << std::endl;
            tokens.push_back(std::move(token));
        }
        SkipWhitespace();
    }

    if (errors > 0)
        std::cerr << "[Lexer] Tokenization completed with " << errors << " errors" << std::endl;

    return !errors;
}

std::vector<std::unique_ptr<Token>> &Lexer::GetTokens()
{
    return tokens;
}
