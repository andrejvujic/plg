#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>

#include "lexer/tokens/token.hpp"
#include "parser/ast/ast.hpp"
#include "parser/ast/program.hpp"

#include "parser/ast/statements/statement.hpp"
#include "parser/ast/statements/var_declaration_statement.hpp"
#include "parser/ast/statements/return_statement.hpp"
#include "parser/ast/statements/if_statement.hpp"
#include "parser/ast/statements/for_statement.hpp"
#include "parser/ast/statements/expression_statement.hpp"

#include "parser/ast/expressions/expression.hpp"
#include "parser/ast/expressions/assignment_expression.hpp"

class Parser
{
private:
    std::unique_ptr<Program> ast;
    const std::vector<std::unique_ptr<const Token>> &tokens;
    size_t index = 0;

    bool HasMoreTokens() const;
    bool NextMatches(TokenType type) const;
    bool NextMatchesAny(const std::initializer_list<TokenType> &types) const;

    std::unique_ptr<const Function> ParseFunction();
    std::unique_ptr<const FunctionParameter> ParseFunctionParameter();

    std::unique_ptr<const ComplexStatement> ParseComplexStatement();
    std::unique_ptr<const Statement> ParseStatement();
    std::unique_ptr<const Statement> ParseVarDeclarationOrAssignment();
    std::unique_ptr<const VarDeclarationStatement> ParseVarDeclarationStatement();
    std::unique_ptr<const ExpressionStatement> ParseVarAssignmentStatement();
    std::unique_ptr<const IfStatement> ParseIfStatement();
    std::unique_ptr<const ReturnStatement> ParseReturnStatement();
    std::unique_ptr<const ForStatement> ParseForStatement();

    std::unique_ptr<const Expression> ParseExpression();
    std::unique_ptr<const Expression> ParsePrimaryExpression();
    std::unique_ptr<const AssignmentExpression> ParseVarAssignmentExpression();

    const std::unique_ptr<const Token> &Consume(TokenType type);
    const std::unique_ptr<const Token> &ConsumeAny(std::initializer_list<TokenType> types);
    void ConsumeAll(std::initializer_list<TokenType> types);

    void PrintUnexpectedToken(const std::unique_ptr<const Token> &token) const;
    void PrintExpectedType(TokenType type) const;
    void PrintExpectedAnyOfTypes(const std::initializer_list<TokenType> &types) const;

public:
    Parser(const std::vector<std::unique_ptr<const Token>> &t);

    void Parse();

    const std::unique_ptr<Program> &GetAst() const;
};

#endif
