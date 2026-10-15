#include "parser/parser.hpp"

#include "shared/types.hpp"

#include "ast/ast.hpp"
#include "ast/program.hpp"
#include "ast/statements/functions/function_declaration.hpp"
#include "ast/expressions/num_literal_expression.hpp"
#include "ast/expressions/bool_literal_expression.hpp"
#include "ast/expressions/id_expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/assignment_expression.hpp"
#include "ast/expressions/call_expression.hpp"

#include <iostream>

void Parser::Parse()
{
    ast = std::make_unique<Program>();
    while (HasMoreTokens())
    {
        auto statement = ParseStatement();
        ast->AddStatement(std::move(statement));
    }
}

const std::unique_ptr<Program> &Parser::GetAst() const
{
    return ast;
}

bool Parser::HasMoreTokens() const
{
    return index < tokens.size();
}

bool Parser::NextMatches(TokenType type) const
{
    if (!HasMoreTokens())
        return false;

    const std::unique_ptr<Token> &token = tokens.at(index);
    return token->Matches(type);
}

bool Parser::NextMatchesAny(const std::initializer_list<TokenType> &types) const
{
    if (!HasMoreTokens())
        return false;

    const std::unique_ptr<Token> &token = tokens.at(index);
    return token->MatchesAny(types);
}

const std::unique_ptr<Token> &Parser::Consume(TokenType type)
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedType(type);
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<Token> &token = tokens.at(index);
    if (!token->Matches(type))
    {
        PrintUnexpectedToken(token);
        PrintExpectedType(type);
        exit(EXIT_FAILURE);
    }

    ++index;
    return token;
}

const std::unique_ptr<Token> &Parser::ConsumeAny(std::initializer_list<TokenType> types)
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedAnyOfTypes(types);
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<Token> &token = tokens.at(index);
    if (!token->MatchesAny(types))
    {
        PrintUnexpectedToken(token);
        PrintExpectedAnyOfTypes(types);
        exit(EXIT_FAILURE);
    }

    ++index;
    return token;
}

void Parser::ConsumeAll(std::initializer_list<TokenType> types)
{
    for (auto type : types)
        Consume(type);
}

std::unique_ptr<FunctionDeclaration> Parser::ParseFunctionDeclaration()
{
    auto &startToken = Consume(TokenType::KEYWORD_FUNCTION);
    auto &nameToken = Consume(TokenType::ID);

    std::vector<std::unique_ptr<FunctionParameter>> parameters;
    Consume(TokenType::O_PAREN);
    while (!NextMatches(TokenType::C_PAREN))
    {
        parameters.push_back(ParseFunctionParameter());

        if (NextMatches(TokenType::COMMA))
        {
            Consume(TokenType::COMMA);
            continue;
        }

        if (!NextMatches(TokenType::C_PAREN))
        {
            auto &token = tokens.at(index);
            PrintUnexpectedToken(token);
            PrintExpectedType(TokenType::COMMA);
            exit(EXIT_FAILURE);
        }
    }

    ConsumeAll({TokenType::C_PAREN, TokenType::ARROW});

    auto &returnType = Consume(TokenType::KEYWORD_TYPE);
    auto body = ParseComplexStatement();
    auto function = std::make_unique<FunctionDeclaration>(
        startToken->GetLocation(),
        nameToken->GetLexeme(),
        returnType->GetLexeme(),
        std::move(body));

    for (auto &parameter : parameters)
        function->AddParameter(std::move(parameter));

    return function;
}

std::unique_ptr<FunctionParameter> Parser::ParseFunctionParameter()
{
    auto &parameterTypeToken = Consume(TokenType::KEYWORD_TYPE);
    auto &parameterNameToken = Consume(TokenType::ID);
    return std::make_unique<FunctionParameter>(
        parameterTypeToken->GetLocation(),
        parameterTypeToken->GetLexeme(),
        parameterNameToken->GetLexeme());
}

std::unique_ptr<ComplexStatement> Parser::ParseComplexStatement()
{
    const auto &startToken = Consume(TokenType::O_CURLY);
    auto complexStatement = std::make_unique<ComplexStatement>(startToken->GetLocation());

    while (true)
    {
        if (NextMatches(TokenType::C_CURLY))
        {
            Consume(TokenType::C_CURLY);
            return complexStatement;
        }

        auto statement = ParseStatement();
        complexStatement->AddStatement(std::move(statement));
    }
}

std::unique_ptr<Statement> Parser::ParseStatement()
{
    if (NextMatches(TokenType::KEYWORD_FUNCTION))
        return ParseFunctionDeclaration();

    if (NextMatches(TokenType::KEYWORD_TYPE))
        return ParseVariableDeclaration();

    if (NextMatches(TokenType::KEYWORD_IF))
        return ParseIfStatement();

    if (NextMatches(TokenType::KEYWORD_RETURN))
        return ParseReturnStatement();

    if (NextMatches(TokenType::KEYWORD_FOR))
        return ParseForStatement();

    auto expression = ParseExpression();
    Consume(TokenType::SEMICOLON);
    const auto location = expression->GetLocation();
    return std::make_unique<ExpressionStatement>(location, std::move(expression));
}

std::unique_ptr<VariableDeclaration> Parser::ParseVariableDeclaration()
{
    auto &varTypeToken = Consume(TokenType::KEYWORD_TYPE);
    auto &varNameToken = Consume(TokenType::ID);

    if (NextMatches(TokenType::SEMICOLON))
    {
        Consume(TokenType::SEMICOLON);
        return std::make_unique<VariableDeclaration>(
            varTypeToken->GetLocation(),
            varNameToken->GetLexeme(),
            varTypeToken->GetLexeme());
    }

    Consume(TokenType::ASSIGNMENT);
    auto initialValue = ParseExpression();
    Consume(TokenType::SEMICOLON);

    return std::make_unique<VariableDeclaration>(
        varTypeToken->GetLocation(),
        varNameToken->GetLexeme(),
        varTypeToken->GetLexeme(),
        std::move(initialValue));
}

std::unique_ptr<IfStatement> Parser::ParseIfStatement()
{
    const auto &startToken = Consume(TokenType::KEYWORD_IF);
    Consume(TokenType::O_PAREN);

    auto condition = ParseExpression();
    Consume(TokenType::C_PAREN);

    std::unique_ptr<Statement> consequent;
    if (NextMatches(TokenType::O_CURLY))
        consequent = ParseComplexStatement();
    else
        consequent = ParseStatement();

    if (!NextMatches(TokenType::KEYWORD_ELSE))
        return std::make_unique<IfStatement>(
            startToken->GetLocation(),
            std::move(condition),
            std::move(consequent));

    Consume(TokenType::KEYWORD_ELSE);

    std::unique_ptr<Statement> alternate;
    if (NextMatches(TokenType::KEYWORD_IF))
        alternate = ParseIfStatement();
    else if (NextMatches(TokenType::O_CURLY))
        alternate = ParseComplexStatement();
    else
        alternate = ParseStatement();

    return std::make_unique<IfStatement>(
        startToken->GetLocation(),
        std::move(condition),
        std::move(consequent),
        std::move(alternate));
}

std::unique_ptr<ReturnStatement> Parser::ParseReturnStatement()
{
    const auto &startToken = Consume(TokenType::KEYWORD_RETURN);

    if (!NextMatchesAny({TokenType::ID, TokenType::NUMBER}))
    {
        Consume(TokenType::SEMICOLON);
        return std::make_unique<ReturnStatement>(startToken->GetLocation());
    }

    auto expression = ParseExpression();
    Consume(TokenType::SEMICOLON);
    return std::make_unique<ReturnStatement>(startToken->GetLocation(), std::move(expression));
}

std::unique_ptr<ForStatement> Parser::ParseForStatement()
{
    const auto &startToken = Consume(TokenType::KEYWORD_FOR);
    Consume(TokenType::O_PAREN);

    auto init = ParseExpression();
    Consume(TokenType::SEMICOLON);

    auto condition = ParseExpression();
    Consume(TokenType::SEMICOLON);

    auto update = ParseExpression();
    Consume(TokenType::C_PAREN);

    std::unique_ptr<Statement> body;
    if (NextMatches(TokenType::O_CURLY))
        body = ParseComplexStatement();
    else
        body = ParseStatement();

    return std::make_unique<ForStatement>(
        startToken->GetLocation(),
        std::move(init),
        std::move(condition),
        std::move(update),
        std::move(body));
}

std::unique_ptr<Expression> Parser::ParseExpression()
{
    std::unique_ptr<Expression> left;

    if (NextMatches(TokenType::O_PAREN))
        left = ParseGroupedExpression();
    else
        left = ParsePrimaryExpression();

    if (NextMatches(TokenType::ASSIGNMENT))
    {
        Consume(TokenType::ASSIGNMENT);
        auto right = ParseExpression();
        const auto location = left->GetLocation();
        return std::make_unique<AssignmentExpression>(
            location,
            std::move(left),
            std::move(right));
    }

    std::unique_ptr<Expression> right;
    auto opTokens = {TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR, TokenType::LOGICAL_OPERATOR};
    while (NextMatchesAny(opTokens) || NextMatches(TokenType::C_PAREN))
    {
        if (NextMatches(TokenType::C_PAREN))
            return left;

        const auto &opToken = ConsumeAny(opTokens);
        if (NextMatches(TokenType::O_PAREN))
            right = ParseGroupedExpression();
        else if (opToken->GetLexeme() == BinaryExpression::Mul || opToken->GetLexeme() == BinaryExpression::Div)
            right = ParsePrimaryExpression();
        else
            right = ParseExpression();

        const auto location = left->GetLocation();
        left = std::make_unique<BinaryExpression>(
            location,
            opToken->GetLexeme(),
            std::move(left),
            std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::ParseGroupedExpression()
{
    Consume(TokenType::O_PAREN);
    auto expression = ParseExpression();
    Consume(TokenType::C_PAREN);
    return expression;
}

std::unique_ptr<Expression> Parser::ParsePrimaryExpression()
{
    if (NextMatchesAny({TokenType::KEYWORD_TRUE, TokenType::KEYWORD_FALSE}))
    {
        auto &valueToken = ConsumeAny({TokenType::KEYWORD_TRUE, TokenType::KEYWORD_FALSE});
        return std::make_unique<BoolLiteralExpression>(
            valueToken->GetLocation(),
            valueToken->GetLexeme());
    }

    if (NextMatches(TokenType::ID))
    {
        auto &idToken = Consume(TokenType::ID);

        if (NextMatches(TokenType::O_PAREN))
        {
            Consume(TokenType::O_PAREN);

            auto calleeExpression = std::make_unique<IdExpression>(
                idToken->GetLocation(),
                idToken->GetLexeme());

            auto callExpression = std::make_unique<CallExpression>(
                idToken->GetLocation(),
                std::move(calleeExpression));

            while (!NextMatches(TokenType::C_PAREN))
            {
                auto argument = ParseExpression();
                callExpression->AddArgument(std::move(argument));

                if (NextMatches(TokenType::COMMA))
                {
                    Consume(TokenType::COMMA);
                    continue;
                }

                if (!NextMatches(TokenType::C_PAREN))
                {
                    auto &token = tokens.at(index);
                    PrintUnexpectedToken(token);
                    PrintExpectedType(TokenType::COMMA);
                    exit(EXIT_FAILURE);
                }
            }

            Consume(TokenType::C_PAREN);
            return callExpression;
        }

        return std::make_unique<IdExpression>(
            idToken->GetLocation(),
            idToken->GetLexeme());
    }

    if (NextMatches(TokenType::NUMBER))
    {
        auto &valueToken = Consume(TokenType::NUMBER);

        auto &value = valueToken->GetLexeme();
        auto const length = value.size();
        if (value.at(length - 1) == 'u')
            return std::make_unique<NumLiteralExpression>(
                valueToken->GetLocation(),
                value.substr(0, length - 1),
                Types::UsignedInt);

        return std::make_unique<NumLiteralExpression>(
            valueToken->GetLocation(),
            value,
            Types::Int);
    }

    auto &token = tokens.at(index);
    PrintUnexpectedToken(token);
    exit(EXIT_FAILURE);
}

void Parser::PrintUnexpectedToken(const std::unique_ptr<Token> &token) const
{
    const auto location = token->GetLocation();
    std::cerr << "[Parser] ERROR: line " << location.line << " column " << location.column
              << ": Unexpected token " << token->GetType()
              << " (\"" << token->GetLexeme() << "\")" << std::endl;
}

void Parser::PrintExpectedType(TokenType type) const
{
    std::cerr << "[Parser] Expected -> " << type << std::endl;
}

void Parser::PrintExpectedAnyOfTypes(const std::initializer_list<TokenType> &types) const
{
    std::cerr << "[Parser] Expected any of -> ";

    size_t currentTypeIndex = 0;
    std::cerr << "{";
    for (auto type : types)
    {
        std::cerr << type;
        if (currentTypeIndex++ < types.size() - 1)
            std::cerr << ", ";
    }
    std::cerr << "}" << std::endl;
}
