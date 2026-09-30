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

    bool AnalyzeStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    bool AnalyzeExpressionStatement(const std::unique_ptr<const Statement> &statement);
    bool AnalyzeIfStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    bool AnalyzeReturnStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    bool AnalyzeForStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    bool AnalyzeComplexStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function);
    bool AnalyzeVarDeclarationStatement(const std::unique_ptr<const Statement> &statement);

    bool AnalyzeExpression(const std::unique_ptr<const Expression> &expression);
    bool AnalyzeIdExpression(const std::unique_ptr<const Expression> &expression);
    bool AnalyzeNumLiteralExpression(const std::unique_ptr<const Expression> &expression);
    bool AnalyzeFunctionCallExpression(const std::unique_ptr<const Expression> &expression);
    bool AnalyzeAssignmentExpression(const std::unique_ptr<const Expression> &expression);
    bool AnalyzeBinaryExpression(const std::unique_ptr<const Expression> &expression);

    std::string ResolveExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetIdExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetNumLiteralExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string GetFunctionCallExpressionType(const std::unique_ptr<const Expression> &expression);
    std::string ResolveBinaryExpressionType(const std::unique_ptr<const Expression> &expression);

    std::ostream &PrintErrorPrefix(std::ostream &out, const AstNode *node) const;

public:
    SemanticAnalyzer();

    void AnalyzeProgram(const std::unique_ptr<Program> &program);
};

#endif