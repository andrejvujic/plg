#ifndef SEMANTIC_ANALYZER_HPP_INCLUDED
#define SEMANTIC_ANALYZER_HPP_INCLUDED

#include "semantic_analysis/scope.hpp"

#include "ast/program.hpp"
#include "ast/function/function.hpp"

class SemanticAnalyzer
{
private:
    std::shared_ptr<Scope> currentScope;

    void AnalyzeFunction(const std::unique_ptr<const Function> &function);
    void AnalyzeExpression(const std::unique_ptr<const Expression> &expression);

    void AnalyzeStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    void AnalyzeExpressionStatement(const std::unique_ptr<const Statement> &statement);
    void AnalyzeIfStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    void AnalyzeReturnStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    void AnalyzeForStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    void AnalyzeComplexStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    void AnalyzeVarDeclarationStatement(const std::unique_ptr<const Statement> &statement);

    void AnalyzeIdExpression(const std::unique_ptr<const Expression> &expression);
    void AnalyzeNumLiteralExpression(const std::unique_ptr<const Expression> &expression);
    void AnalyzeFunctionCallExpression(const std::unique_ptr<const Expression> &expression);
    void AnalyzeAssignmentExpression(const std::unique_ptr<const Expression> &expression);
    void AnalyzeBinaryExpression(const std::unique_ptr<const Expression> &expression);

    std::string ResolveExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetIdExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetNumLiteralExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetFunctionCallExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string ResolveBinaryExpressionType(const std::unique_ptr<const Expression> &expression);

public:
    SemanticAnalyzer();

    void AnalyzeProgram(const std::unique_ptr<Program> &program);
};

#endif