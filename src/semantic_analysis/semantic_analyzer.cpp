#include "semantic_analysis/semantic_analyzer.hpp"
#include "semantic_analysis/symbols/function_symbol.hpp"
#include "semantic_analysis/symbols/variable_symbol.hpp"
#include "semantic_analysis/types.hpp"

#include "ast/statements/if_statement.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/for_statement.hpp"
#include "ast/statements/var_declaration_statement.hpp"
#include "ast/statements/expression_statement.hpp"

#include "ast/expressions/id_expression.hpp"
#include "ast/expressions/num_literal_expression.hpp"
#include "ast/expressions/function_call_expression.hpp"
#include "ast/expressions/assignment_expression.hpp"
#include "ast/expressions/binary_expression.hpp"

#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() : currentScope(std::make_shared<Scope>())
{
}

void SemanticAnalyzer::AnalyzeProgram(const std::unique_ptr<Program> &program)
{
    for (auto &function : program->GetFunctions())
    {
        FunctionSymbol f(function->GetName(), function->GetReturnType());
        for (auto &parameter : function->GetParameters())
            f.AddParameter(parameter->GetVarType());

        std::unique_ptr<const Symbol> symbol = std::make_unique<FunctionSymbol>(f);
        currentScope->DeclareSymbol(symbol);
    }

    for (auto &function : program->GetFunctions())
    {
        std::shared_ptr<Scope> functionScope = std::make_shared<Scope>(currentScope);
        currentScope = functionScope;
        AnalyzeFunction(function);
    }
}

void SemanticAnalyzer::AnalyzeFunction(const std::unique_ptr<const Function> &function)
{
    for (auto &parameter : function->GetParameters())
    {
        if (!currentScope->SymbolExists(parameter->GetVarName(), SymbolKind::Variable))
        {
            std::unique_ptr<const Symbol> symbol = std::make_unique<VariableSymbol>(
                parameter->GetVarName(),
                parameter->GetVarType());
            currentScope->DeclareSymbol(symbol);
        }
        else
            std::cerr << "[SemanticAnalyzer] ERROR: Redefinition of variable \"" << parameter->GetVarName() << "\"" << std::endl;
    }

    auto &body = function->GetBody();
    for (auto &statement : body->GetStatements())
        AnalyzeStatement(statement, function);
}

void SemanticAnalyzer::AnalyzeStatement(
    const std::unique_ptr<const Statement> &statement,
    const std::unique_ptr<const Function> &function)
{
    switch (statement->GetType())
    {
    case AstNodeType::IfStatement:
        AnalyzeIfStatement(statement, function);
        break;
    case AstNodeType::ForStatement:
        AnalyzeForStatement(statement, function);
        break;
    case AstNodeType::ReturnStatement:
        AnalyzeReturnStatement(statement, function);
        break;
    case AstNodeType::ComplexStatement:
        AnalyzeComplexStatement(statement, function);
        break;
    case AstNodeType::ExpressionStatement:
        AnalyzeExpressionStatement(statement);
        break;
    case AstNodeType::VarDeclartaionStatement:
        AnalyzeVarDeclarationStatement(statement);
        break;
    default:
        std::cerr << "[SemanticAnalyzer] ERROR: Semantic analysis not implemented for given statement type" << std::endl;
        break;
    }
}

void SemanticAnalyzer::AnalyzeComplexStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto complexStatement = dynamic_cast<const ComplexStatement *>(statement.get());
    for (auto &childStatement : complexStatement->GetStatements())
        AnalyzeStatement(childStatement, function);
}

void SemanticAnalyzer::AnalyzeIfStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto ifStatement = dynamic_cast<const IfStatement *>(statement.get());
    AnalyzeExpression(ifStatement->GetCondition());

    auto consequentScope = std::make_shared<Scope>(currentScope);
    currentScope = consequentScope;
    AnalyzeStatement(ifStatement->GetConsequentStatement(), function);

    currentScope = consequentScope->GetParent();

    if (!ifStatement->HasAlternateStatement())
        return;

    auto alternateScope = std::make_shared<Scope>(currentScope);
    currentScope = alternateScope;
    AnalyzeStatement(ifStatement->GetAlternateStatement(), function);

    currentScope = alternateScope->GetParent();
}

void SemanticAnalyzer::AnalyzeForStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto forStatement = dynamic_cast<const ForStatement *>(statement.get());

    AnalyzeExpression(forStatement->GetInitExpression()); // TODO: This needs to be an AssignmentExpression.
    AnalyzeExpression(forStatement->GetCondition());
    AnalyzeExpression(forStatement->GetUpdateExpression()); // TODO: This needs to be an AssignmentExpression.

    auto bodyScope = std::make_shared<Scope>(currentScope);
    currentScope = bodyScope;
    AnalyzeStatement(forStatement->GetBody(), function);

    currentScope = bodyScope->GetParent();
}

void SemanticAnalyzer::AnalyzeExpressionStatement(const std::unique_ptr<const Statement> &statement)
{
    auto expressionStatement = dynamic_cast<const ExpressionStatement *>(statement.get());
    AnalyzeExpression(expressionStatement->GetExpression());
}

void SemanticAnalyzer::AnalyzeReturnStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto returnStatement = dynamic_cast<const ReturnStatement *>(statement.get());

    if (!returnStatement->HasValue() && function->GetReturnType() == Types::Void)
        return;

    if (!returnStatement->HasValue() && function->GetReturnType() != Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Non-void function \""
                  << function->GetName() << "\" must return a value"
                  << std::endl;
        return;
    }

    if (returnStatement->HasValue() && function->GetReturnType() == Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Void function \""
                  << function->GetName() << "\" can't return a value"
                  << std::endl;
        return;
    }

    const auto returnType = ResolveExpressionType(returnStatement->GetValue());
    if (returnType == function->GetReturnType())
        return;

    std::cerr << "[SemanticAnalyzer] ERROR: Invalid return type, expected \""
              << function->GetReturnType() << "\" but got \""
              << returnType << "\""
              << std::endl;
}

void SemanticAnalyzer::AnalyzeVarDeclarationStatement(const std::unique_ptr<const Statement> &statement)
{
    auto declarationStatement = dynamic_cast<const VarDeclarationStatement *>(statement.get());

    if (declarationStatement->GetVarType() == Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Variable \""
                  << declarationStatement->GetVarName() << "\" can't have type \""
                  << declarationStatement->GetVarType() << "\""
                  << std::endl;
        return;
    }

    if (currentScope->SymbolExists(declarationStatement->GetVarName(), SymbolKind::Variable))
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Redefinition of variable \""
                  << declarationStatement->GetVarName() << "\""
                  << std::endl;
        return;
    }

    if (declarationStatement->HasInitialValue())
    {
        const auto initialValueType = ResolveExpressionType(declarationStatement->GetInitialValue());
        if (initialValueType != declarationStatement->GetVarType())
            std::cerr << "[SemanticAnalyzer] ERROR: Invalid initial value of \""
                      << declarationStatement->GetVarName() << "\", expected \""
                      << declarationStatement->GetVarType() << "\" but got \""
                      << initialValueType << "\""
                      << std::endl;
    }

    std::unique_ptr<const Symbol> symbol = std::make_unique<VariableSymbol>(
        declarationStatement->GetVarName(),
        declarationStatement->GetVarType());
    currentScope->DeclareSymbol(symbol);
}

void SemanticAnalyzer::AnalyzeExpression(const std::unique_ptr<const Expression> &expression)
{
    switch (expression->GetType())
    {
    case AstNodeType::IdExpression:
        AnalyzeIdExpression(expression);
        break;
    case AstNodeType::NumLiteralExpression:
        break;
    case AstNodeType::BinaryExpression:
        AnalyzeBinaryExpression(expression);
        break;
    case AstNodeType::FunctionCallExpression:
        AnalyzeFunctionCallExpression(expression);
        break;
    case AstNodeType::AssignmentExpression:
        AnalyzeAssignmentExpression(expression);
        break;
    default:
        std::cerr << "[SemanticAnalyzer] ERROR: Semantic analysis not implemented for given expression" << std::endl;
        break;
    }
}

void SemanticAnalyzer::AnalyzeIdExpression(const std::unique_ptr<const Expression> &expression)
{
    const IdExpression *e = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentScope->SymbolExists(e->GetValue(), SymbolKind::Variable))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Variable \"" << e->GetValue()
            << "\" doesn't exist"
            << std::endl;
        return;
    }
}

void SemanticAnalyzer::AnalyzeFunctionCallExpression(const std::unique_ptr<const Expression> &expression)
{
    const FunctionCallExpression *call = dynamic_cast<const FunctionCallExpression *>(expression.get());
    if (!currentScope->SymbolExists(call->GetFunction(), SymbolKind::Function))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Function \"" << call->GetFunction()
            << "\" doesn't exist"
            << std::endl;
        return;
    }

    auto &s = currentScope->GetSymbol(call->GetFunction(), SymbolKind::Function);
    auto function = dynamic_cast<const FunctionSymbol *>(s.get());
    if (function->GetParameterCount() != call->GetArgumentCount())
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Invalid number of arguments, expected "
            << function->GetParameterCount() << " but got " << call->GetArgumentCount()
            << std::endl;
        return;
    }

    auto &parameters = function->GetParameters();
    auto &arguments = call->GetArguments();

    auto paramIterator = parameters.begin();
    auto argIterator = arguments.begin();
    size_t index = 0;
    for (; paramIterator != parameters.end() && argIterator != arguments.end();
         ++paramIterator, ++argIterator, ++index)
    {
        const auto parameterType = *paramIterator;
        const auto argumentType = ResolveExpressionType(*argIterator);

        if (parameterType != argumentType)
            std::cerr
                << "[SemanticAnalyzer] ERROR: Positional argument at position " << index
                << " is of wrong type, expected \"" << parameterType << " \" but got \"" << argumentType << "\""
                << std::endl;
    }
}

void SemanticAnalyzer::AnalyzeAssignmentExpression(const std::unique_ptr<const Expression> &expression)
{
    if (expression->GetType() != AstNodeType::AssignmentExpression)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Invalid expression type, expected AssignmentExpression" << std::endl;
        return;
    }

    auto assignment = dynamic_cast<const AssignmentExpression *>(expression.get());
    if (!currentScope->SymbolExists(assignment->GetVarName(), SymbolKind::Variable))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Variable \""
            << assignment->GetVarValue() << "\" doesn't exist"
            << std::endl;
        return;
    }

    auto &s = currentScope->GetSymbol(assignment->GetVarName(), SymbolKind::Variable);
    AnalyzeExpression(assignment->GetVarValue());

    auto valueType = ResolveExpressionType(assignment->GetVarValue());
    if (!s->IsOfType(valueType))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Can't assign value of type \""
            << valueType << "\" to variable of type \""
            << s->GetType() << "\""
            << std::endl;
    }
}

void SemanticAnalyzer::AnalyzeBinaryExpression(const std::unique_ptr<const Expression> &expression)
{
    const BinaryExpression *e = dynamic_cast<const BinaryExpression *>(expression.get());
    AnalyzeExpression(e->GetLeft());
    AnalyzeExpression(e->GetRight());
}

std::string SemanticAnalyzer::ResolveExpressionType(const std::unique_ptr<const Expression> &expression)
{
    switch (expression->GetType())
    {
    case AstNodeType::IdExpression:
        return GetIdExpressionType(expression);
    case AstNodeType::NumLiteralExpression:
        return GetNumLiteralExpressionType(expression);
    case AstNodeType::BinaryExpression:
        return ResolveBinaryExpressionType(expression);
    case AstNodeType::FunctionCallExpression:
        return GetFunctionCallExpressionType(expression);
    default:
        std::cerr << "[SemanticAnalyzer] ERROR: Can't resolve type for given expression" << std::endl;
        return Types::Unknown;
    }
}

std::string SemanticAnalyzer::GetIdExpressionType(const std::unique_ptr<const Expression> &expression)
{
    auto idExpression = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentScope->SymbolExists(idExpression->GetValue(), SymbolKind::Variable))
        return Types::Unknown;

    auto &symbol = currentScope->GetSymbol(idExpression->GetValue(), SymbolKind::Variable);
    return symbol->GetType();
}

std::string SemanticAnalyzer::GetNumLiteralExpressionType(const std::unique_ptr<const Expression> &expression)
{
    auto e = dynamic_cast<const NumLiteralExpression *>(expression.get());
    return e->GetLiteralType();
}

std::string SemanticAnalyzer::GetFunctionCallExpressionType(const std::unique_ptr<const Expression> &expression)
{
    auto functionCallExpression = dynamic_cast<const FunctionCallExpression *>(expression.get());
    if (!currentScope->SymbolExists(functionCallExpression->GetFunction(), SymbolKind::Function))
        return Types::Unknown;

    auto &symbol = currentScope->GetSymbol(functionCallExpression->GetFunction(), SymbolKind::Function);
    return symbol->GetType();
}

std::string SemanticAnalyzer::ResolveBinaryExpressionType(const std::unique_ptr<const Expression> &expression)
{
    auto e = dynamic_cast<const BinaryExpression *>(expression.get());

    auto leftType = ResolveExpressionType(e->GetLeft());
    if (leftType == Types::Unknown)
        return Types::Unknown;

    auto rightType = ResolveExpressionType(e->GetRight());
    if (rightType == Types::Unknown)
        return Types::Unknown;

    return leftType == rightType ? rightType : Types::Unknown;
}