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
    auto &startToken = Consume(TokenType::KeywordFunction);
    auto &nameToken = Consume(TokenType::Id);

    std::vector<std::unique_ptr<FunctionParameter>> parameters;
    Consume(TokenType::OParen);
    while (!NextMatches(TokenType::CParen))
    {
        parameters.push_back(ParseFunctionParameter());

        if (NextMatches(TokenType::Comma))
        {
            Consume(TokenType::Comma);
            continue;
        }

        if (!NextMatches(TokenType::CParen))
        {
            auto &token = tokens.at(index);
            PrintUnexpectedToken(token);
            PrintExpectedType(TokenType::Comma);
            exit(EXIT_FAILURE);
        }
    }

    ConsumeAll({TokenType::CParen, TokenType::Arrow});

    auto &returnType = Consume(TokenType::KeywordType);
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
    auto &parameterTypeToken = Consume(TokenType::KeywordType);
    auto &parameterNameToken = Consume(TokenType::Id);
    return std::make_unique<FunctionParameter>(
        parameterTypeToken->GetLocation(),
        parameterTypeToken->GetLexeme(),
        parameterNameToken->GetLexeme());
}

std::unique_ptr<ComplexStatement> Parser::ParseComplexStatement()
{
    const auto &startToken = Consume(TokenType::OCurly);
    auto complexStatement = std::make_unique<ComplexStatement>(startToken->GetLocation());

    while (true)
    {
        if (NextMatches(TokenType::CCurly))
        {
            Consume(TokenType::CCurly);
            return complexStatement;
        }

        auto statement = ParseStatement();
        complexStatement->AddStatement(std::move(statement));
    }
}

std::unique_ptr<Statement> Parser::ParseStatement()
{
    if (NextMatches(TokenType::KeywordFunction))
        return ParseFunctionDeclaration();

    if (NextMatches(TokenType::KeywordType))
        return ParseVariableDeclaration();

    if (NextMatches(TokenType::KeywordIf))
        return ParseIfStatement();

    if (NextMatches(TokenType::KeywordReturn))
        return ParseReturnStatement();

    if (NextMatches(TokenType::KeywordFor))
        return ParseForStatement();

    if (NextMatches(TokenType::KeywordWhile))
        return ParseWhileStatement();

    auto expression = ParseExpression();
    Consume(TokenType::Semicolon);
    const auto location = expression->GetLocation();
    return std::make_unique<ExpressionStatement>(location, std::move(expression));
}

std::unique_ptr<VariableDeclaration> Parser::ParseVariableDeclaration()
{
    auto &varTypeToken = Consume(TokenType::KeywordType);
    auto &varNameToken = Consume(TokenType::Id);

    if (NextMatches(TokenType::Semicolon))
    {
        Consume(TokenType::Semicolon);
        return std::make_unique<VariableDeclaration>(
            varTypeToken->GetLocation(),
            varNameToken->GetLexeme(),
            varTypeToken->GetLexeme());
    }

    Consume(TokenType::Assignment);
    auto initialValue = ParseExpression();
    Consume(TokenType::Semicolon);

    return std::make_unique<VariableDeclaration>(
        varTypeToken->GetLocation(),
        varNameToken->GetLexeme(),
        varTypeToken->GetLexeme(),
        std::move(initialValue));
}

std::unique_ptr<IfStatement> Parser::ParseIfStatement()
{
    const auto &startToken = Consume(TokenType::KeywordIf);
    Consume(TokenType::OParen);

    auto condition = ParseExpression();
    Consume(TokenType::CParen);

    std::unique_ptr<Statement> consequent;
    if (NextMatches(TokenType::OCurly))
        consequent = ParseComplexStatement();
    else
        consequent = ParseStatement();

    if (!NextMatches(TokenType::KeywordElse))
        return std::make_unique<IfStatement>(
            startToken->GetLocation(),
            std::move(condition),
            std::move(consequent));

    Consume(TokenType::KeywordElse);

    std::unique_ptr<Statement> alternate;
    if (NextMatches(TokenType::KeywordIf))
        alternate = ParseIfStatement();
    else if (NextMatches(TokenType::OCurly))
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
    const auto &startToken = Consume(TokenType::KeywordReturn);

    if (!NextMatchesAny({TokenType::Id, TokenType::Number}))
    {
        Consume(TokenType::Semicolon);
        return std::make_unique<ReturnStatement>(startToken->GetLocation());
    }

    auto expression = ParseExpression();
    Consume(TokenType::Semicolon);
    return std::make_unique<ReturnStatement>(startToken->GetLocation(), std::move(expression));
}

std::unique_ptr<ForStatement> Parser::ParseForStatement()
{
    const auto &startToken = Consume(TokenType::KeywordFor);
    Consume(TokenType::OParen);

    auto init = ParseExpression();
    Consume(TokenType::Semicolon);

    auto condition = ParseExpression();
    Consume(TokenType::Semicolon);

    auto update = ParseExpression();
    Consume(TokenType::CParen);

    std::unique_ptr<Statement> body;
    if (NextMatches(TokenType::OCurly))
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

std::unique_ptr<WhileStatement> Parser::ParseWhileStatement()
{
    auto &token = Consume(TokenType::KeywordWhile);
    Consume(TokenType::OParen);
    auto condition = ParseExpression();
    Consume(TokenType::CParen);

    std::unique_ptr<Statement> body;
    if (NextMatches(TokenType::OCurly))
        body = ParseComplexStatement();
    else
        body = ParseStatement();

    return std::make_unique<WhileStatement>(
        token->GetLocation(),
        std::move(condition),
        std::move(body));
}

std::unique_ptr<Expression> Parser::ParseExpression()
{
    std::unique_ptr<Expression> left;

    if (NextMatches(TokenType::OParen))
        left = ParseGroupedExpression();
    else
        left = ParsePrimaryExpression();

    if (NextMatches(TokenType::Assignment))
    {
        Consume(TokenType::Assignment);
        auto right = ParseExpression();
        const auto location = left->GetLocation();
        return std::make_unique<AssignmentExpression>(
            location,
            std::move(left),
            std::move(right));
    }

    std::unique_ptr<Expression> right;
    auto opTokens = {TokenType::ArithmeticOperator, TokenType::RelationalOperator, TokenType::LogicalOperator};
    while (NextMatchesAny(opTokens) || NextMatches(TokenType::CParen))
    {
        if (NextMatches(TokenType::CParen))
            return left;

        const auto &opToken = ConsumeAny(opTokens);
        if (NextMatches(TokenType::OParen))
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
    Consume(TokenType::OParen);
    auto expression = ParseExpression();
    Consume(TokenType::CParen);
    return expression;
}

std::unique_ptr<Expression> Parser::ParsePrimaryExpression()
{
    if (NextMatchesAny({TokenType::KeywordTrue, TokenType::KeywordFalse}))
    {
        auto &valueToken = ConsumeAny({TokenType::KeywordTrue, TokenType::KeywordFalse});
        return std::make_unique<BoolLiteralExpression>(
            valueToken->GetLocation(),
            valueToken->GetLexeme());
    }

    if (NextMatches(TokenType::Id))
    {
        auto &idToken = Consume(TokenType::Id);

        if (NextMatches(TokenType::OParen))
        {
            Consume(TokenType::OParen);

            auto calleeExpression = std::make_unique<IdExpression>(
                idToken->GetLocation(),
                idToken->GetLexeme());

            auto callExpression = std::make_unique<CallExpression>(
                idToken->GetLocation(),
                std::move(calleeExpression));

            while (!NextMatches(TokenType::CParen))
            {
                auto argument = ParseExpression();
                callExpression->AddArgument(std::move(argument));

                if (NextMatches(TokenType::Comma))
                {
                    Consume(TokenType::Comma);
                    continue;
                }

                if (!NextMatches(TokenType::CParen))
                {
                    auto &token = tokens.at(index);
                    PrintUnexpectedToken(token);
                    PrintExpectedType(TokenType::Comma);
                    exit(EXIT_FAILURE);
                }
            }

            Consume(TokenType::CParen);
            return callExpression;
        }

        return std::make_unique<IdExpression>(
            idToken->GetLocation(),
            idToken->GetLexeme());
    }

    if (NextMatches(TokenType::Number))
    {
        auto &valueToken = Consume(TokenType::Number);

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
