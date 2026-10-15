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
#include "ast/expressions/function_call_expression.hpp"

#include <iostream>

Parser::Parser(const std::vector<std::unique_ptr<const Token>> &t) : ast(std::make_unique<Program>()), tokens(t)
{
}

void Parser::Parse()
{
    while (HasMoreTokens())
    {
        auto statement = ParseStatement();
        ast->AddStatement(statement);
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

    const std::unique_ptr<const Token> &token = tokens.at(index);
    return token->Matches(type);
}

bool Parser::NextMatchesAny(const std::initializer_list<TokenType> &types) const
{
    if (!HasMoreTokens())
        return false;

    const std::unique_ptr<const Token> &token = tokens.at(index);
    return token->MatchesAny(types);
}

const std::unique_ptr<const Token> &Parser::Consume(TokenType type)
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedType(type);
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    if (!token->Matches(type))
    {
        PrintUnexpectedToken(token);
        PrintExpectedType(type);
        exit(EXIT_FAILURE);
    }

    ++index;
    return token;
}

const std::unique_ptr<const Token> &Parser::ConsumeAny(std::initializer_list<TokenType> types)
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedAnyOfTypes(types);
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
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

std::unique_ptr<const FunctionDeclaration> Parser::ParseFunctionDeclaration()
{
    auto function = std::make_unique<FunctionDeclaration>();

    auto &startToken = Consume(TokenType::KEYWORD_FUNCTION);
    function->SetLocation(startToken->GetLocation());

    auto &nameToken = Consume(TokenType::ID);
    function->SetName(nameToken->GetLexeme());

    Consume(TokenType::O_PAREN);
    while (!NextMatches(TokenType::C_PAREN))
    {
        auto parameter = ParseFunctionParameter();
        function->AddParameter(parameter);

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
    function->SetReturnType(returnType->GetLexeme());

    auto body = ParseComplexStatement();
    function->SetBody(body);

    return function;
}

std::unique_ptr<const FunctionParameter> Parser::ParseFunctionParameter()
{
    auto parameter = std::make_unique<FunctionParameter>();

    auto &parameterTypeToken = Consume(TokenType::KEYWORD_TYPE);
    parameter->SetLocation(parameterTypeToken->GetLocation());
    parameter->SetVarType(parameterTypeToken->GetLexeme());

    auto &parameterNameToken = Consume(TokenType::ID);
    parameter->SetVarName(parameterNameToken->GetLexeme());

    return parameter;
}

std::unique_ptr<const ComplexStatement> Parser::ParseComplexStatement()
{
    auto complexStatement = std::make_unique<ComplexStatement>();
    const auto &startToken = Consume(TokenType::O_CURLY);
    complexStatement->SetLocation(startToken->GetLocation());

    while (true)
    {
        if (NextMatches(TokenType::C_CURLY))
        {
            Consume(TokenType::C_CURLY);
            return complexStatement;
        }

        auto statement = ParseStatement();
        complexStatement->AddStatement(statement);
    }

    return complexStatement;
}

std::unique_ptr<const Statement> Parser::ParseStatement()
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

    std::unique_ptr<ExpressionStatement> statement = std::make_unique<ExpressionStatement>();
    auto expression = ParseExpression();
    Consume(TokenType::SEMICOLON);
    statement->SetLocation(expression->GetLocation());
    statement->SetExpression(expression);
    return statement;

    // auto &token = tokens.at(index);
    // PrintUnexpectedToken(token);
    // exit(EXIT_FAILURE);
}

std::unique_ptr<const VariableDeclaration> Parser::ParseVariableDeclaration()

{
    auto statement = std::make_unique<VariableDeclaration>();

    auto &varTypeToken = Consume(TokenType::KEYWORD_TYPE);
    statement->SetLocation(varTypeToken->GetLocation());
    statement->SetVarType(varTypeToken->GetLexeme());

    auto &varNameToken = Consume(TokenType::ID);
    statement->SetVarName(varNameToken->GetLexeme());

    if (NextMatches(TokenType::SEMICOLON))
    {
        Consume(TokenType::SEMICOLON);
        return statement;
    }

    Consume(TokenType::ASSIGNMENT);
    auto expression = ParseExpression();
    statement->SetInitialVarValue(expression);
    Consume(TokenType::SEMICOLON);
    return statement;
}

std::unique_ptr<const IfStatement> Parser::ParseIfStatement()
{
    auto statement = std::make_unique<IfStatement>();

    const auto &startToken = Consume(TokenType::KEYWORD_IF);
    statement->SetLocation(startToken->GetLocation());
    Consume(TokenType::O_PAREN);

    auto condition = ParseExpression();
    statement->SetCondition(condition);

    Consume(TokenType::C_PAREN);

    if (NextMatches(TokenType::O_CURLY))
    {
        std::unique_ptr<const Statement> consequent = ParseComplexStatement();
        statement->SetConsequentStatement(consequent);
    }
    else
    {
        auto consequent = ParseStatement();
        statement->SetConsequentStatement(consequent);
    }

    if (!NextMatches(TokenType::KEYWORD_ELSE))
        return statement;

    Consume(TokenType::KEYWORD_ELSE);

    if (NextMatches(TokenType::KEYWORD_IF))
    {
        std::unique_ptr<const Statement> alternate = ParseIfStatement();
        statement->SetAlternateStatement(alternate);
        return statement;
    }

    if (NextMatches(TokenType::O_CURLY))
    {
        std::unique_ptr<const Statement> alternate = ParseComplexStatement();
        statement->SetAlternateStatement(alternate);
    }
    else
    {
        auto alternate = ParseStatement();
        statement->SetAlternateStatement(alternate);
    }

    return statement;
}

std::unique_ptr<const ReturnStatement> Parser::ParseReturnStatement()
{
    auto statement = std::make_unique<ReturnStatement>();
    const auto &startToken = Consume(TokenType::KEYWORD_RETURN);
    statement->SetLocation(startToken->GetLocation());

    if (!NextMatchesAny({TokenType::ID, TokenType::NUMBER}))
    {
        Consume(TokenType::SEMICOLON);
        return statement;
    }

    auto expression = ParseExpression();
    statement->SetValue(expression);

    Consume(TokenType::SEMICOLON);
    return statement;
}

std::unique_ptr<const ForStatement> Parser::ParseForStatement()
{
    auto statement = std::make_unique<ForStatement>();

    const auto &startToken = Consume(TokenType::KEYWORD_FOR);
    statement->SetLocation(startToken->GetLocation());
    Consume(TokenType::O_PAREN);

    std::unique_ptr<const Expression> init = ParseExpression();
    statement->SetInitExpression(init);

    Consume(TokenType::SEMICOLON);

    auto condition = ParseExpression();
    statement->SetCondition(condition);

    Consume(TokenType::SEMICOLON);

    std::unique_ptr<const Expression> update = ParseExpression();
    statement->SetUpdateExpression(update);

    Consume(TokenType::C_PAREN);

    if (NextMatches(TokenType::O_CURLY))
    {
        std::unique_ptr<const Statement> body = ParseComplexStatement();
        statement->SetBody(body);
    }
    else
    {
        auto body = ParseStatement();
        statement->SetBody(body);
    }

    return statement;
}

std::unique_ptr<const Expression> Parser::ParseExpression()
{
    std::unique_ptr<const Expression> left;

    if (NextMatches(TokenType::O_PAREN))
        left = ParseGroupedExpression();
    else
        left = ParsePrimaryExpression();

    if (NextMatches(TokenType::ASSIGNMENT))
    {
        Consume(TokenType::ASSIGNMENT);
        auto right = ParseExpression();
        auto assignmentExpression = std::make_unique<AssignmentExpression>();
        assignmentExpression->SetLocation(left->GetLocation());
        assignmentExpression->SetAssignee(left);
        assignmentExpression->SetValue(right);
        return assignmentExpression;
    }

    std::unique_ptr<const Expression> right;
    auto opTokens = {TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR, TokenType::LOGICAL_OPERATOR};
    while (NextMatchesAny(opTokens) || NextMatches(TokenType::C_PAREN))
    {
        if (NextMatches(TokenType::C_PAREN))
            return left;

        const auto &opToken = ConsumeAny(opTokens);
        if (NextMatches(TokenType::O_PAREN))
            right = ParseGroupedExpression();
        else if (opToken->GetLexeme() == BinaryExpression::OP_MUL || opToken->GetLexeme() == BinaryExpression::OP_DIV)
            right = ParsePrimaryExpression();
        else
            right = ParseExpression();

        auto tmp = std::make_unique<BinaryExpression>();
        tmp->SetLocation(left->GetLocation());
        tmp->SetOperator(opToken->GetLexeme());
        tmp->SetLeft(left);
        tmp->SetRight(right);
        left = std::move(tmp);
    }

    return left;
}

std::unique_ptr<const Expression> Parser::ParseGroupedExpression()
{
    Consume(TokenType::O_PAREN);
    auto expression = ParseExpression();
    Consume(TokenType::C_PAREN);
    return expression;
}

std::unique_ptr<const Expression> Parser::ParsePrimaryExpression()
{
    if (NextMatchesAny({TokenType::KEYWORD_TRUE, TokenType::KEYWORD_FALSE}))
    {
        auto &valueToken = ConsumeAny({TokenType::KEYWORD_TRUE, TokenType::KEYWORD_FALSE});
        auto expression = std::make_unique<BoolLiteralExpression>();
        expression->SetLocation(valueToken->GetLocation());
        expression->SetRawValue(valueToken->GetLexeme());
        return expression;
    }

    if (NextMatches(TokenType::ID))
    {
        auto &idToken = Consume(TokenType::ID);

        if (NextMatches(TokenType::O_PAREN))
        {
            Consume(TokenType::O_PAREN);
            auto functionCall = std::make_unique<FunctionCallExpression>();
            functionCall->SetLocation(idToken->GetLocation());
            functionCall->SetFunction(idToken->GetLexeme());

            while (!NextMatches(TokenType::C_PAREN))
            {
                auto argument = ParseExpression();
                functionCall->AddArgument(argument);

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
            return functionCall;
        }

        auto expression = std::make_unique<IdExpression>();
        expression->SetLocation(idToken->GetLocation());
        expression->SetValue(idToken->GetLexeme());
        return expression;
    }

    if (NextMatches(TokenType::NUMBER))
    {
        auto &valueToken = Consume(TokenType::NUMBER);

        auto &value = valueToken->GetLexeme();
        auto expression = std::make_unique<NumLiteralExpression>();
        expression->SetLocation(valueToken->GetLocation());

        auto const length = value.size();
        if (value.at(length - 1) == 'u')
        {
            expression->SetRawValue(value.substr(0, length - 1));
            expression->SetLiteralType(Types::UsignedInt);
        }
        else
        {
            expression->SetRawValue(value);
            expression->SetLiteralType(Types::Int);
        }

        return expression;
    }

    auto &token = tokens.at(index);
    PrintUnexpectedToken(token);
    exit(EXIT_FAILURE);
}

void Parser::PrintUnexpectedToken(const std::unique_ptr<const Token> &token) const
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
