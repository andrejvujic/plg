#ifndef SEMANTIC_ANALYZER_HPP_INCLUDED
#define SEMANTIC_ANALYZER_HPP_INCLUDED

#include "semantic_analysis/scope.hpp"
#include "semantic_analysis/symbols/function_symbol.hpp"

#include "ast/program.hpp"
#include "ast/statements/functions/function_declaration.hpp"

#include <stack>

class SemanticAnalyzer
{
private:
    std::shared_ptr<Scope> currentScope;
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
    bool AnalyzeFunctionCallExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeAssignmentExpression(const std::unique_ptr<Expression> &expression);
    bool AnalyzeBinaryExpression(const std::unique_ptr<Expression> &expression);

    std::string ResolveExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetIdExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetNumLiteralExpressionType(const std::unique_ptr<Expression> &expression);
    std::string GetFunctionCallExpressionType(const std::unique_ptr<Expression> &expression);
    std::string ResolveBinaryExpressionType(const std::unique_ptr<Expression> &expression);

    std::ostream &PrintErrorPrefix(std::ostream &out, const AstNode *node) const;

public:
    SemanticAnalyzer();

    void AnalyzeProgram(const std::unique_ptr<Program> &program);
};

#endif
