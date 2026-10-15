#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>

#include "lexer/tokens/token.hpp"
#include "ast/ast.hpp"
#include "ast/program.hpp"

#include "ast/statements/statement.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/if_statement.hpp"
#include "ast/statements/for_statement.hpp"
#include "ast/statements/expression_statement.hpp"
#include "ast/statements/functions/function_declaration.hpp"

#include "ast/expressions/expression.hpp"
#include "ast/expressions/assignment_expression.hpp"

class Parser
{
private:
    std::unique_ptr<Program> ast;
    const std::vector<std::unique_ptr<Token>> &tokens;
    size_t index = 0;

    bool HasMoreTokens() const;
    bool NextMatches(TokenType type) const;
    bool NextMatchesAny(const std::initializer_list<TokenType> &types) const;

    std::unique_ptr<FunctionDeclaration> ParseFunctionDeclaration();
    std::unique_ptr<FunctionParameter> ParseFunctionParameter();

    std::unique_ptr<ComplexStatement> ParseComplexStatement();
    std::unique_ptr<Statement> ParseStatement();
    std::unique_ptr<VariableDeclaration> ParseVariableDeclaration();
    std::unique_ptr<IfStatement> ParseIfStatement();
    std::unique_ptr<ReturnStatement> ParseReturnStatement();
    std::unique_ptr<ForStatement> ParseForStatement();

    std::unique_ptr<Expression> ParseExpression();
    std::unique_ptr<Expression> ParseGroupedExpression();
    std::unique_ptr<Expression> ParsePrimaryExpression();

    const std::unique_ptr<Token> &Consume(TokenType type);
    const std::unique_ptr<Token> &ConsumeAny(std::initializer_list<TokenType> types);
    void ConsumeAll(std::initializer_list<TokenType> types);

    void PrintUnexpectedToken(const std::unique_ptr<Token> &token) const;
    void PrintExpectedType(TokenType type) const;
    void PrintExpectedAnyOfTypes(const std::initializer_list<TokenType> &types) const;

public:
    Parser(const std::vector<std::unique_ptr<Token>> &t);

    void Parse();

    const std::unique_ptr<Program> &GetAst() const;
};

#endif
