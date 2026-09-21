#include "parser.hpp"

#include <iostream>

Parser::Parser(const std::vector<std::unique_ptr<const Token>> &t) : tokens(t)
{
}

bool Parser::Parse()
{
    while (HasMoreTokens())
    {
        if (!ParseFunction())
            return false;
    }
    return true;
}

bool Parser::HasMoreTokens() const
{
    return index < tokens.size();
}

bool Parser::NextMatches(TokenType type) const
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    return token->Matches(type);
}

bool Parser::NextMatchesAny(std::initializer_list<TokenType> types) const
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    return token->MatchesAny(types);
}

bool Parser::Consume(TokenType type)
{
    if (!HasMoreTokens())
    {
        std::cout << "[Parser] ERROR: Unexpected EOF" << std::endl;
        return false;
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    if (!token->Matches(type))
    {
        std::cout << "[Parser] ERROR: Unexpected token " << token->GetType() << " (\"" << token->GetLexeme() << "\")" << " at line " << token->GetLineNumber() << " column " << token->GetColumnNumber() << ", expected " << type << std::endl;
        return false;
    }

    ++index;
    return true;
}

bool Parser::ConsumeAny(std::initializer_list<TokenType> types)
{
    if (!HasMoreTokens())
    {
        std::cout << "[Parser] ERROR: Unexpected EOF" << std::endl;
        return false;
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    if (!token->MatchesAny(types))
    {
        std::cout << "[Parser] ERROR: Unexpected token " << token->GetType() << " (\"" << token->GetLexeme() << "\")" << " at line " << token->GetLineNumber() << " column " << token->GetColumnNumber() << ", expected any of ";

        size_t currentTypeIndex = 0;
        std::cout << "{";
        for (auto type : types)
        {
            std::cout << type;
            if (currentTypeIndex++ < types.size() - 1)
                std::cout << ", ";
        }
        std::cout << "}" << std::endl;

        return false;
    }

    ++index;
    return true;
}

bool Parser::ParseFunction()
{
    if (!Consume(TokenType::KEYWORD_FUNCTION))
        return false;
    if (!Consume(TokenType::ID))
        return false;
    if (!Consume(TokenType::O_PAREN))
        return false;
    if (!Consume(TokenType::C_PAREN))
        return false;
    if (!Consume(TokenType::ARROW))
        return false;
    if (!Consume(TokenType::KEYWORD_TYPE))
        return false;

    if (!ParseComplexStatement())
        return false;

    return true;
}

bool Parser::ParseComplexStatement()
{
    if (!Consume(TokenType::O_CURLY))
        return false;

    while (true)
    {
        if (NextMatches(TokenType::C_CURLY))
        {
            Consume(TokenType::C_CURLY);
            return true;
        }

        if (!ParseStatement())
            return false;
    }

    return false;
}

bool Parser::ParseStatement()
{
    if (NextMatches(TokenType::ID))
        return ParseAssignmentStatement();

    if (NextMatches(TokenType::KEYWORD_IF))
        return ParseIfStatement();

    if (NextMatches(TokenType::KEYWORD_RETURN))
        return ParseReturnStatement();

    return false;
}

bool Parser::ParseAssignmentStatement()
{
    if (!Consume(TokenType::ID))
        return false;

    if (NextMatches(TokenType::COLON))
    {
        if (!Consume(TokenType::COLON))
            return false;
        if (!Consume(TokenType::KEYWORD_TYPE))
            return false;
        if (NextMatches(TokenType::SEMICOLON))
            return Consume(TokenType::SEMICOLON);
    }

    if (!Consume(TokenType::ASSIGNMENT))
        return false;

    if (!ParseExpression())
        return false;

    return Consume(TokenType::SEMICOLON);
}

bool Parser::ParseIfStatement()
{
    if (!Consume(TokenType::KEYWORD_IF))
        return false;
    if (!Consume(TokenType::O_PAREN))
        return false;
    if (!ParseExpression())
        return false;
    if (!Consume(TokenType::C_PAREN))
        return false;

    if (NextMatches(TokenType::O_CURLY))
        return ParseComplexStatement();

    return ParseStatement();
}

bool Parser::ParseReturnStatement()
{
    if (!Consume(TokenType::KEYWORD_RETURN))
        return false;

    if (!NextMatchesAny({TokenType::ID, TokenType::NUM_LITERAL}))
        return Consume(TokenType::SEMICOLON);

    if (!ParseExpression())
        return false;

    return Consume(TokenType::SEMICOLON);
}

bool Parser::ParseExpression()
{
    if (!ConsumeAny({TokenType::ID, TokenType::NUM_LITERAL}))
        return false;

    if (NextMatchesAny({TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR}))
    {
        if (!ConsumeAny({TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR}))
            return false;
        return ParseExpression();
    }

    return true;
}