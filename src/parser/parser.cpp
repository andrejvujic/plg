#include "parser/parser.hpp"
#include "ast/ast.hpp"
#include "ast/program.hpp"
#include "ast/function/function.hpp"
#include "ast/expressions/num_literal_expression.hpp"
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
        auto function = ParseFunction();
        ast->AddFunction(function);
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
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedType(type);
        exit(EXIT_FAILURE);
    }

    const std::unique_ptr<const Token> &token = tokens.at(index);
    return token->Matches(type);
}

bool Parser::NextMatchesAny(const std::initializer_list<TokenType> &types) const
{
    if (!HasMoreTokens())
    {
        std::cerr << "[Parser] ERROR: Unexpected EOF" << std::endl;
        PrintExpectedAnyOfTypes(types);
        exit(EXIT_FAILURE);
    }

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

std::unique_ptr<const Function> Parser::ParseFunction()
{
    auto function = std::make_unique<Function>();

    Consume(TokenType::KEYWORD_FUNCTION);

    auto &name = Consume(TokenType::ID);
    function->SetName(name->GetLexeme());

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

    auto &parameterType = Consume(TokenType::KEYWORD_TYPE);
    parameter->SetVarType(parameterType->GetLexeme());

    auto &parameterName = Consume(TokenType::ID);
    parameter->SetVarName(parameterName->GetLexeme());

    return parameter;
}

std::unique_ptr<const ComplexStatement> Parser::ParseComplexStatement()
{
    auto complexStatement = std::make_unique<ComplexStatement>();
    Consume(TokenType::O_CURLY);
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
    if (NextMatches(TokenType::KEYWORD_TYPE))
        return ParseVarDeclarationStatement();

    if (NextMatches(TokenType::ID))
        return ParseVarAssignmentStatement();

    if (NextMatches(TokenType::KEYWORD_IF))
        return ParseIfStatement();

    if (NextMatches(TokenType::KEYWORD_RETURN))
        return ParseReturnStatement();

    if (NextMatches(TokenType::KEYWORD_FOR))
        return ParseForStatement();

    auto &token = tokens.at(index);
    PrintUnexpectedToken(token);
    exit(EXIT_FAILURE);
}

std::unique_ptr<const Statement> Parser::ParseVarDeclarationOrAssignment()
{
    if (NextMatches(TokenType::KEYWORD_TYPE))
        return ParseVarDeclarationStatement();

    return ParseVarAssignmentStatement();
}

std::unique_ptr<const VarDeclarationStatement> Parser::ParseVarDeclarationStatement()

{
    auto statement = std::make_unique<VarDeclarationStatement>();

    auto &varType = Consume(TokenType::KEYWORD_TYPE);
    statement->SetVarType(varType->GetLexeme());

    auto &varName = Consume(TokenType::ID);
    statement->SetVarName(varName->GetLexeme());

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

std::unique_ptr<const ExpressionStatement> Parser::ParseVarAssignmentStatement()
{
    auto statement = std::make_unique<ExpressionStatement>();

    std::unique_ptr<const Expression> expression = ParseVarAssignmentExpression();
    statement->SetExpression(expression);

    Consume(TokenType::SEMICOLON);

    return statement;
}

std::unique_ptr<const AssignmentExpression> Parser::ParseVarAssignmentExpression()
{
    auto assignmentExpression = std::make_unique<AssignmentExpression>();

    auto &varName = Consume(TokenType::ID);
    assignmentExpression->SetVarName(varName->GetLexeme());

    Consume(TokenType::ASSIGNMENT);
    auto expression = ParseExpression();
    assignmentExpression->SetVarValue(expression);
    return assignmentExpression;
}

std::unique_ptr<const IfStatement> Parser::ParseIfStatement()
{
    auto statement = std::make_unique<IfStatement>();

    ConsumeAll({TokenType::KEYWORD_IF, TokenType::O_PAREN});

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
    Consume(TokenType::KEYWORD_RETURN);

    if (!NextMatchesAny({TokenType::ID, TokenType::NUM_LITERAL}))
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

    ConsumeAll({TokenType::KEYWORD_FOR, TokenType::O_PAREN});

    auto init = ParseVarAssignmentExpression();
    statement->SetInitExpression(init);

    Consume(TokenType::SEMICOLON);

    auto condition = ParseExpression();
    statement->SetCondition(condition);

    Consume(TokenType::SEMICOLON);

    auto update = ParseVarAssignmentExpression();
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
    std::unique_ptr<const Expression> left = ParsePrimaryExpression();
    std::unique_ptr<const Expression> right;

    while (NextMatchesAny({TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR}))
    {
        const auto &op = ConsumeAny({TokenType::ARITHMETIC_OPERATOR, TokenType::RELATIONAL_OPERATOR});
        if (op->GetLexeme() == BinaryExpression::OP_MUL || op->GetLexeme() == BinaryExpression::OP_DIV)
            right = ParsePrimaryExpression();
        else
            right = ParseExpression();

        auto tmp = std::make_unique<BinaryExpression>();
        tmp->SetOperator(op->GetLexeme());
        tmp->SetLeft(left);
        tmp->SetRight(right);
        left = std::move(tmp);
    }

    return left;
}

std::unique_ptr<const Expression> Parser::ParsePrimaryExpression()
{
    if (NextMatches(TokenType::ID))
    {
        auto &id = Consume(TokenType::ID);

        if (NextMatches(TokenType::O_PAREN))
        {
            Consume(TokenType::O_PAREN);
            auto functionCall = std::make_unique<FunctionCallExpression>();
            functionCall->SetFunction(id->GetLexeme());

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
        expression->SetValue(id->GetLexeme());
        return expression;
    }

    if (NextMatches(TokenType::NUM_LITERAL))
    {
        auto &literal = Consume(TokenType::NUM_LITERAL);
        auto expression = std::make_unique<NumLiteralExpression>();
        expression->SetRawValue(literal->GetLexeme());
        return expression;
    }

    auto &token = tokens.at(index);
    PrintUnexpectedToken(token);
    exit(EXIT_FAILURE);
}

void Parser::PrintUnexpectedToken(const std::unique_ptr<const Token> &token) const
{
    std::cerr << "[Parser] ERROR: Unexpected token " << token->GetType() << " (\"" << token->GetLexeme() << "\")" << " at line " << token->GetLineNumber() << " column " << token->GetColumnNumber() << std::endl;
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
