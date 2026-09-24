#include "parser/parser.hpp"
#include "parser/ast/ast.hpp"
#include "parser/ast/program.hpp"
#include "parser/ast/expressions/num_literal_expression.hpp"
#include "parser/ast/expressions/id_expression.hpp"
#include "parser/ast/expressions/binary_expression.hpp"

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

    ConsumeAll({TokenType::O_PAREN, TokenType::C_PAREN, TokenType::ARROW});

    auto &returnType = Consume(TokenType::KEYWORD_TYPE);
    function->SetReturnType(returnType->GetLexeme());

    auto body = ParseComplexStatement();
    function->SetBody(body);

    return function;
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
    if (NextMatches(TokenType::ID))
        return ParseVarDeclarationOrAssignmentStatement();

    if (NextMatches(TokenType::KEYWORD_IF))
        return ParseIfStatement();

    if (NextMatches(TokenType::KEYWORD_RETURN))
        return ParseReturnStatement();

    auto &token = tokens.at(index);
    PrintUnexpectedToken(token);
    exit(EXIT_FAILURE);
}

std::unique_ptr<const Statement> Parser::ParseVarDeclarationOrAssignmentStatement()
{
    auto &varName = Consume(TokenType::ID);

    if (NextMatches(TokenType::COLON))
        return ParseVarDeclarationStatement(varName);

    return ParseVarAssignmentStatement(varName);
}

std::unique_ptr<const VarDeclarationStatement> Parser::ParseVarDeclarationStatement(const std::unique_ptr<const Token> &varName)
{
    auto statement = std::make_unique<VarDeclarationStatement>();
    statement->SetVarName(varName->GetLexeme());

    Consume(TokenType::COLON);
    auto &varType = Consume(TokenType::KEYWORD_TYPE);
    statement->SetVarType(varType->GetLexeme());

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

std::unique_ptr<const VarAssignmentStatement> Parser::ParseVarAssignmentStatement(const std::unique_ptr<const Token> &varName)
{
    auto statement = std::make_unique<VarAssignmentStatement>();
    statement->SetVarName(varName->GetLexeme());

    Consume(TokenType::ASSIGNMENT);
    auto expression = ParseExpression();
    statement->SetVarValue(expression);

    Consume(TokenType::SEMICOLON);

    return statement;
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
