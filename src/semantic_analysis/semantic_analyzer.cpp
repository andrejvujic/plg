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

bool SemanticAnalyzer::AnalyzeStatement(
    const std::unique_ptr<const Statement> &statement,
    const std::unique_ptr<const Function> &function)
{
    switch (statement->GetType())
    {
    case AstNodeType::IfStatement:
        return AnalyzeIfStatement(statement, function);
    case AstNodeType::ForStatement:
        return AnalyzeForStatement(statement, function);
    case AstNodeType::ReturnStatement:
        return AnalyzeReturnStatement(statement, function);
    case AstNodeType::ComplexStatement:
        return AnalyzeComplexStatement(statement, function);
    case AstNodeType::ExpressionStatement:
        return AnalyzeExpressionStatement(statement);
    case AstNodeType::VarDeclartaionStatement:
        return AnalyzeVarDeclarationStatement(statement);
    default:
        std::cerr << "[SemanticAnalyzer] ERROR: Semantic analysis not implemented for given statement type" << std::endl;
        return false;
        break;
    }
}

bool SemanticAnalyzer::AnalyzeComplexStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    bool result = true;
    auto complexStatement = dynamic_cast<const ComplexStatement *>(statement.get());
    for (auto &childStatement : complexStatement->GetStatements())
        result = result && AnalyzeStatement(childStatement, function);
    return result;
}

bool SemanticAnalyzer::AnalyzeIfStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    bool result = true;
    auto ifStatement = dynamic_cast<const IfStatement *>(statement.get());
    result = result && AnalyzeExpression(ifStatement->GetCondition());

    auto consequentScope = std::make_shared<Scope>(currentScope);
    currentScope = consequentScope;
    result = result && AnalyzeStatement(ifStatement->GetConsequentStatement(), function);

    currentScope = consequentScope->GetParent();

    if (!ifStatement->HasAlternateStatement())
        return result;

    auto alternateScope = std::make_shared<Scope>(currentScope);
    currentScope = alternateScope;
    result = result && AnalyzeStatement(ifStatement->GetAlternateStatement(), function);

    currentScope = alternateScope->GetParent();

    return result;
}

bool SemanticAnalyzer::AnalyzeForStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto forStatement = dynamic_cast<const ForStatement *>(statement.get());

    bool result = true;
    result = result && AnalyzeExpression(forStatement->GetInitExpression()); // TODO: This needs to be an AssignmentExpression.
    result = result && AnalyzeExpression(forStatement->GetCondition());
    result = result && AnalyzeExpression(forStatement->GetUpdateExpression()); // TODO: This needs to be an AssignmentExpression.

    auto bodyScope = std::make_shared<Scope>(currentScope);
    currentScope = bodyScope;
    result = result && AnalyzeStatement(forStatement->GetBody(), function);

    currentScope = bodyScope->GetParent();
    return result;
}

bool SemanticAnalyzer::AnalyzeExpressionStatement(const std::unique_ptr<const Statement> &statement)
{
    auto expressionStatement = dynamic_cast<const ExpressionStatement *>(statement.get());
    return AnalyzeExpression(expressionStatement->GetExpression());
}

bool SemanticAnalyzer::AnalyzeReturnStatement(const std::unique_ptr<const Statement> &statement, const std::unique_ptr<const Function> &function)
{
    auto returnStatement = dynamic_cast<const ReturnStatement *>(statement.get());

    if (!returnStatement->HasValue() && function->GetReturnType() == Types::Void)
        return true;

    if (!returnStatement->HasValue() && function->GetReturnType() != Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Non-void function \""
                  << function->GetName() << "\" must return a value"
                  << std::endl;
        return false;
    }

    if (returnStatement->HasValue() && function->GetReturnType() == Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Void function \""
                  << function->GetName() << "\" can't return a value"
                  << std::endl;
        return false;
    }

    const auto returnType = ResolveExpressionType(returnStatement->GetValue());
    if (returnType == function->GetReturnType())
        return true;

    std::cerr << "[SemanticAnalyzer] ERROR: Invalid return type, expected \""
              << function->GetReturnType() << "\" but got \""
              << returnType << "\""
              << std::endl;
    return false;
}

bool SemanticAnalyzer::AnalyzeVarDeclarationStatement(const std::unique_ptr<const Statement> &statement)
{
    auto declarationStatement = dynamic_cast<const VarDeclarationStatement *>(statement.get());

    if (declarationStatement->GetVarType() == Types::Void)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Variable \""
                  << declarationStatement->GetVarName() << "\" can't have type \""
                  << declarationStatement->GetVarType() << "\""
                  << std::endl;
        return false;
    }

    if (currentScope->SymbolExists(declarationStatement->GetVarName(), SymbolKind::Variable))
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Redefinition of variable \""
                  << declarationStatement->GetVarName() << "\""
                  << std::endl;
        return false;
    }

    std::unique_ptr<const Symbol> symbol = std::make_unique<VariableSymbol>(
        declarationStatement->GetVarName(),
        declarationStatement->GetVarType());
    currentScope->DeclareSymbol(symbol);

    if (declarationStatement->HasInitialValue())
    {
        const auto initialValueType = ResolveExpressionType(declarationStatement->GetInitialValue());
        if (initialValueType == declarationStatement->GetVarType())
            return true;

        std::cerr << "[SemanticAnalyzer] ERROR: Invalid initial value of \""
                  << declarationStatement->GetVarName() << "\", expected \""
                  << declarationStatement->GetVarType() << "\" but got \""
                  << initialValueType << "\""
                  << std::endl;
        return false;
    }

    return true;
}

bool SemanticAnalyzer::AnalyzeExpression(const std::unique_ptr<const Expression> &expression)
{
    switch (expression->GetType())
    {
    case AstNodeType::IdExpression:
        return AnalyzeIdExpression(expression);
    case AstNodeType::NumLiteralExpression:
        return true;
    case AstNodeType::BinaryExpression:
        return AnalyzeBinaryExpression(expression);
    case AstNodeType::FunctionCallExpression:
        return AnalyzeFunctionCallExpression(expression);
    case AstNodeType::AssignmentExpression:
        return AnalyzeAssignmentExpression(expression);
    default:
        std::cerr << "[SemanticAnalyzer] ERROR: Semantic analysis not implemented for given expression" << std::endl;
        return false;
    }
}

bool SemanticAnalyzer::AnalyzeIdExpression(const std::unique_ptr<const Expression> &expression)
{
    const IdExpression *e = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentScope->SymbolExists(e->GetValue(), SymbolKind::Variable))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Variable \"" << e->GetValue()
            << "\" doesn't exist"
            << std::endl;
        return false;
    }

    return true;
}

bool SemanticAnalyzer::AnalyzeFunctionCallExpression(const std::unique_ptr<const Expression> &expression)
{
    const FunctionCallExpression *call = dynamic_cast<const FunctionCallExpression *>(expression.get());
    if (!currentScope->SymbolExists(call->GetFunction(), SymbolKind::Function))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Function \"" << call->GetFunction()
            << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &s = currentScope->GetSymbol(call->GetFunction(), SymbolKind::Function);
    auto function = dynamic_cast<const FunctionSymbol *>(s.get());
    if (function->GetParameterCount() != call->GetArgumentCount())
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Invalid number of arguments, expected "
            << function->GetParameterCount() << " but got " << call->GetArgumentCount()
            << std::endl;
        return false;
    }

    auto &parameters = function->GetParameters();
    auto &arguments = call->GetArguments();

    auto parameterIterator = parameters.begin();
    auto argumentIterator = arguments.begin();
    size_t index = 0;
    for (; parameterIterator != parameters.end() && argumentIterator != arguments.end(); ++parameterIterator, ++argumentIterator, ++index)
    {
        const auto parameterType = *parameterIterator;
        const auto argumentType = ResolveExpressionType(*argumentIterator);

        if (parameterType == argumentType)
            continue;

        std::cerr
            << "[SemanticAnalyzer] ERROR: Positional argument at position " << index
            << " is of wrong type, expected \"" << parameterType << "\" but got \"" << argumentType << "\""
            << std::endl;

        return false;
    }

    return true;
}

bool SemanticAnalyzer::AnalyzeAssignmentExpression(const std::unique_ptr<const Expression> &expression)
{
    if (expression->GetType() != AstNodeType::AssignmentExpression)
    {
        std::cerr << "[SemanticAnalyzer] ERROR: Invalid expression type, expected AssignmentExpression" << std::endl;
        return false;
    }

    auto assignment = dynamic_cast<const AssignmentExpression *>(expression.get());
    if (!currentScope->SymbolExists(assignment->GetVarName(), SymbolKind::Variable))
    {
        std::cerr
            << "[SemanticAnalyzer] ERROR: Variable \""
            << assignment->GetVarValue() << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &s = currentScope->GetSymbol(assignment->GetVarName(), SymbolKind::Variable);
    if (!AnalyzeExpression(assignment->GetVarValue()))
        return false;

    auto valueType = ResolveExpressionType(assignment->GetVarValue());
    if (s->IsOfType(valueType))
        return true;

    std::cerr
        << "[SemanticAnalyzer] ERROR: Can't assign value of type \""
        << valueType << "\" to variable of type \""
        << s->GetType() << "\""
        << std::endl;
    return false;
}

bool SemanticAnalyzer::AnalyzeBinaryExpression(const std::unique_ptr<const Expression> &expression)
{
    const BinaryExpression *e = dynamic_cast<const BinaryExpression *>(expression.get());
    return AnalyzeExpression(e->GetLeft()) && AnalyzeExpression(e->GetRight());
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
    auto binaryExpression = dynamic_cast<const BinaryExpression *>(expression.get());

    auto leftType = ResolveExpressionType(binaryExpression->GetLeft());
    if (leftType == Types::Unknown)
        return Types::Unknown;

    auto rightType = ResolveExpressionType(binaryExpression->GetRight());
    if (rightType == Types::Unknown)
        return Types::Unknown;

    return leftType == rightType ? rightType : Types::Unknown;
}