#ifndef SEMANTIC_ANALYZER_HPP_INCLUDED
#define SEMANTIC_ANALYZER_HPP_INCLUDED

#include "semantic_analysis/symbol_table.hpp"
#include "semantic_analysis/symbols/function_symbol.hpp"

#include "ast/program.hpp"
#include "ast/statements/functions/function_declaration.hpp"
#include "ast/expressions/call_expression.hpp"

#include <stack>

class SemanticAnalyzer
{
private:
    std::shared_ptr<SymbolTable> currentSymbolTable;
    std::stack<std::string> functionStack;

    bool AnalyzeStatement(const std::unique_ptr<Statement> &statement);

    bool AnalyzeFunctionDeclaration(const std::unique_ptr<Statement> &statement);

    bool AnalyzeVariableDeclaration(const std::unique_ptr<Statement> &statement);
    bool AnalyzeExpressionStatement(const std::unique_ptr<Statement> &statement);
    bool AnalyzeIfStatement(const std::unique_ptr<Statement> &statement);
    bool AnalyzeReturnStatement(const std::unique_ptr<Statement> &statement);
    bool AnalyzeForStatement(const std::unique_ptr<Statement> &statement);
    bool AnalyzeComplexStatement(const std::unique_ptr<Statement> &statement);

    bool AnalyzeExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeIdExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeNumLiteralExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeAssignmentExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeBinaryExpression(const std::unique_ptr<Expression> &expression);

    bool AnalyzeCallExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeNonMemberFunctionCall(const CallExpression *call);
    bool AnalyzeCallArguments(const std::vector<std::unique_ptr<Expression>> &arguments, const std::vector<std::string> &parameterTypes);

    std::string ResolveExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetIdExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetNumLiteralExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetCallExpressionType(const std::unique_ptr<Expression> &expression);
    std::string ResolveBinaryExpressionType(const std::unique_ptr<Expression> &expression);

    std::ostream &PrintErrorPrefix(std::ostream &out, const AstNode *node) const;

public:
    SemanticAnalyzer() : currentSymbolTable(std::make_shared<SymbolTable>()) {}

    void AnalyzeProgram(const std::unique_ptr<Program> &program);
};

#endif
