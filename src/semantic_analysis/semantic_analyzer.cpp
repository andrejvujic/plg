#include "semantic_analysis/semantic_analyzer.hpp"
#include "semantic_analysis/symbols/function_symbol.hpp"
#include "semantic_analysis/symbols/variable_symbol.hpp"
#include "shared/types.hpp"

#include "ast/statements/if_statement.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/for_statement.hpp"
#include "ast/statements/variable_declaration.hpp"
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
    auto result = true;
    for (auto &statement : program->GetStatements())
        result = AnalyzeStatement(statement) && result;

    if (!result)
        std::exit(EXIT_FAILURE);
}

std::unique_ptr<FunctionSymbol> SemanticAnalyzer::CreateFunctionSymbol(const FunctionDeclaration *function)
{
    FunctionSymbol functionSymbol(function->GetName(), function->GetReturnType());
    for (auto &parameter : function->GetParameters())
        functionSymbol.AddParameter(parameter->GetVarType());
    return std::make_unique<FunctionSymbol>(functionSymbol);
}

bool SemanticAnalyzer::AnalyzeFunctionDeclaration(const std::unique_ptr<const Statement> &statement)
{
    auto functionDeclaration = dynamic_cast<const FunctionDeclaration *>(statement.get());
    std::unique_ptr<Symbol> functionSymbol = CreateFunctionSymbol(functionDeclaration);
    currentScope->DeclareSymbol(functionSymbol);

    auto functionScope = std::make_shared<Scope>(currentScope);
    currentScope = functionScope;
    for (auto &parameter : functionDeclaration->GetParameters())
    {
        if (!currentScope->SymbolExists(parameter->GetVarName(), SymbolKind::Variable))
        {
            std::unique_ptr<Symbol> symbol = std::make_unique<VariableSymbol>(
                parameter->GetVarName(),
                parameter->GetVarType());
            currentScope->DeclareSymbol(symbol);
        }
        else
            PrintErrorPrefix(std::cerr, parameter.get())
                << "Redefinition of variable \"" << parameter->GetVarName() << "\""
                << std::endl;
    }

    auto result = true;
    auto &body = functionDeclaration->GetBody();

    functionStack.push(functionDeclaration->GetName());
    for (auto &statement : body->GetStatements())
    {
        auto nextResult = AnalyzeStatement(statement);
        result = result && nextResult;
    }
    functionStack.pop();

    currentScope = currentScope->GetParent();

    return result;
    // TODO: Check if a value is returned from non-void functions
}

bool SemanticAnalyzer::AnalyzeStatement(const std::unique_ptr<const Statement> &statement)
{
    switch (statement->GetType())
    {
    case AstNodeType::VariableDeclaration:
        return AnalyzeVariableDeclaration(statement);
    case AstNodeType::FunctionDeclaration:
        return AnalyzeFunctionDeclaration(statement);
    case AstNodeType::IfStatement:
        return AnalyzeIfStatement(statement);
    case AstNodeType::ForStatement:
        return AnalyzeForStatement(statement);
    case AstNodeType::ReturnStatement:
        return AnalyzeReturnStatement(statement);
    case AstNodeType::ComplexStatement:
        return AnalyzeComplexStatement(statement);
    case AstNodeType::ExpressionStatement:
        return AnalyzeExpressionStatement(statement);
    default:
        PrintErrorPrefix(std::cerr, statement.get()) << "Semantic analysis not implemented for given statement type" << std::endl;
        return false;
        break;
    }
}

bool SemanticAnalyzer::AnalyzeComplexStatement(const std::unique_ptr<const Statement> &statement)
{
    auto result = true;
    auto complexStatement = dynamic_cast<const ComplexStatement *>(statement.get());
    for (auto &childStatement : complexStatement->GetStatements())
    {
        auto nextResult = AnalyzeStatement(childStatement);
        result = result && nextResult;
    }
    return result;
}

bool SemanticAnalyzer::AnalyzeIfStatement(const std::unique_ptr<const Statement> &statement)
{
    auto ifStatement = dynamic_cast<const IfStatement *>(statement.get());

    auto result = AnalyzeExpression(ifStatement->GetCondition());
    auto conditionType = ResolveExpressionType(ifStatement->GetCondition());
    if (conditionType != Types::Bool)
    {
        result = false;
        PrintErrorPrefix(std::cerr, ifStatement)
            << "Invalid type in condition, expected \"bool\" but got \"" << conditionType << "\""
            << std::endl;
    }

    auto consequentScope = std::make_shared<Scope>(currentScope);
    currentScope = consequentScope;
    result = AnalyzeStatement(ifStatement->GetConsequentStatement()) && result;

    currentScope = consequentScope->GetParent();

    if (!ifStatement->HasAlternateStatement())
        return result;

    auto alternateScope = std::make_shared<Scope>(currentScope);
    currentScope = alternateScope;
    result = AnalyzeStatement(ifStatement->GetAlternateStatement()) && result;

    currentScope = alternateScope->GetParent();

    return result;
}

bool SemanticAnalyzer::AnalyzeForStatement(const std::unique_ptr<const Statement> &statement)
{
    auto forStatement = dynamic_cast<const ForStatement *>(statement.get());

    auto result = AnalyzeExpression(forStatement->GetInitExpression()); // TODO: This needs to be an AssignmentExpression.

    result = AnalyzeExpression(forStatement->GetCondition()) && result;
    auto conditionType = ResolveExpressionType(forStatement->GetCondition());
    if (conditionType != Types::Bool)
    {
        result = false;
        PrintErrorPrefix(std::cerr, forStatement)
            << "Invalid type in condition, expected \"bool\" but got \"" << conditionType << "\""
            << std::endl;
    }

    result = AnalyzeExpression(forStatement->GetUpdateExpression()) && result; // TODO: This needs to be an AssignmentExpression.

    auto bodyScope = std::make_shared<Scope>(currentScope);
    currentScope = bodyScope;
    result = AnalyzeStatement(forStatement->GetBody()) && result;

    currentScope = bodyScope->GetParent();
    return result;
}

bool SemanticAnalyzer::AnalyzeExpressionStatement(const std::unique_ptr<const Statement> &statement)
{
    auto expressionStatement = dynamic_cast<const ExpressionStatement *>(statement.get());
    return AnalyzeExpression(expressionStatement->GetExpression());
}

bool SemanticAnalyzer::AnalyzeReturnStatement(const std::unique_ptr<const Statement> &statement)
{
    auto returnStatement = dynamic_cast<const ReturnStatement *>(statement.get());
    if (functionStack.empty() || !currentScope->SymbolExists(functionStack.top(), SymbolKind::Function))
    {
        PrintErrorPrefix(std::cerr, returnStatement)
            << "Unexpected return statement"
            << std::endl;
        return false;
    }

    auto &functionSymbol = currentScope->GetSymbol(functionStack.top(), SymbolKind::Function);

    if (!returnStatement->HasValue())
    {
        if (functionSymbol->GetType() == Types::Void)
            return true;

        PrintErrorPrefix(std::cerr, returnStatement)
            << "Non-void function \""
            << functionSymbol->GetName() << "\" must return a value"
            << std::endl;
        return false;
    }

    if (functionSymbol->GetType() == Types::Void)
    {
        PrintErrorPrefix(std::cerr, returnStatement)
            << "Void function \""
            << functionSymbol->GetName() << "\" can't return a value"
            << std::endl;
        return false;
    }

    const auto returnType = ResolveExpressionType(returnStatement->GetValue());
    if (returnType == functionSymbol->GetType())
        return true;

    PrintErrorPrefix(std::cerr, returnStatement)
        << "Invalid return type, expected \""
        << functionSymbol->GetType() << "\" but got \""
        << returnType << "\""
        << std::endl;

    return false;
}

bool SemanticAnalyzer::AnalyzeVariableDeclaration(const std::unique_ptr<const Statement> &statement)
{
    auto variableDeclaration = dynamic_cast<const VariableDeclaration *>(statement.get());

    if (variableDeclaration->GetVarType() == Types::Void)
    {
        PrintErrorPrefix(std::cerr, variableDeclaration)
            << "Variable \""
            << variableDeclaration->GetVarName() << "\" can't have type \""
            << variableDeclaration->GetVarType() << "\""
            << std::endl;
        return false;
    }

    if (currentScope->SymbolExists(variableDeclaration->GetVarName(), SymbolKind::Variable))
    {
        PrintErrorPrefix(std::cerr, variableDeclaration)
            << "Redefinition of variable \""
            << variableDeclaration->GetVarName() << "\""
            << std::endl;
        return false;
    }

    std::unique_ptr<Symbol> symbol = std::make_unique<VariableSymbol>(
        variableDeclaration->GetVarName(),
        variableDeclaration->GetVarType());
    currentScope->DeclareSymbol(symbol);

    if (variableDeclaration->HasInitialValue())
    {
        auto result = AnalyzeExpression(variableDeclaration->GetInitialValue());
        auto initialValueType = ResolveExpressionType(variableDeclaration->GetInitialValue());
        if (initialValueType == variableDeclaration->GetVarType())
            return result;

        PrintErrorPrefix(std::cerr, variableDeclaration)
            << "Invalid initial value of \""
            << variableDeclaration->GetVarName() << "\", expected \""
            << variableDeclaration->GetVarType() << "\" but got \""
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
    case AstNodeType::BoolLiteralExpression:
        return true;
    case AstNodeType::BinaryExpression:
        return AnalyzeBinaryExpression(expression);
    case AstNodeType::FunctionCallExpression:
        return AnalyzeFunctionCallExpression(expression);
    case AstNodeType::AssignmentExpression:
        return AnalyzeAssignmentExpression(expression);
    default:
        PrintErrorPrefix(std::cerr, expression.get()) << "Semantic analysis not implemented for given expression" << std::endl;
        return false;
    }
}

bool SemanticAnalyzer::AnalyzeIdExpression(const std::unique_ptr<const Expression> &expression)
{
    const IdExpression *e = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentScope->SymbolExists(e->GetValue(), SymbolKind::Variable))
    {
        PrintErrorPrefix(std::cerr, e)
            << "Variable \"" << e->GetValue()
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
        PrintErrorPrefix(std::cerr, call)
            << "Function \"" << call->GetFunction()
            << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &s = currentScope->GetSymbol(call->GetFunction(), SymbolKind::Function);
    auto function = dynamic_cast<const FunctionSymbol *>(s.get());
    if (function->GetParameterCount() != call->GetArgumentCount())
    {
        PrintErrorPrefix(std::cerr, call)
            << "Invalid number of arguments in function call, expected "
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

        PrintErrorPrefix(std::cerr, argumentIterator->get())
            << "Positional argument at position " << index
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
        PrintErrorPrefix(std::cerr, expression.get()) << "Invalid expression type, expected AssignmentExpression" << std::endl;
        return false;
    }

    auto assignmentExpression = dynamic_cast<const AssignmentExpression *>(expression.get());
    auto &assigneeExpression = assignmentExpression->GetAssignee();
    if (assigneeExpression->GetType() != AstNodeType::IdExpression)
    {
        PrintErrorPrefix(std::cerr, assignmentExpression) << "Left hand side isn't assignable" << std::endl;
        return false;
    }

    auto assigneeIdExpression = dynamic_cast<const IdExpression *>(assigneeExpression.get());
    if (!currentScope->SymbolExists(assigneeIdExpression->GetValue(), SymbolKind::Variable))
    {
        PrintErrorPrefix(std::cerr, assignmentExpression)
            << "Variable \""
            << assigneeIdExpression->GetValue() << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &symbol = currentScope->GetSymbol(assigneeIdExpression->GetValue(), SymbolKind::Variable);
    if (!AnalyzeExpression(assignmentExpression->GetValue()))
        return false;

    auto valueType = ResolveExpressionType(assignmentExpression->GetValue());
    if (symbol->IsOfType(valueType))
        return true;

    PrintErrorPrefix(std::cerr, expression.get())
        << "Can't assign value of type \""
        << valueType << "\" to variable \"" << symbol->GetName() << "\" of type \""
        << symbol->GetType() << "\""
        << std::endl;
    return false;
}

bool SemanticAnalyzer::AnalyzeBinaryExpression(const std::unique_ptr<const Expression> &expression)
{
    const BinaryExpression *e = dynamic_cast<const BinaryExpression *>(expression.get());

    const auto leftType = ResolveExpressionType(e->GetLeft());
    const auto rightType = ResolveExpressionType(e->GetRight());

    if (leftType == Types::Unknown || rightType == Types::Unknown)
    {
        const auto leftResult = AnalyzeExpression(e->GetLeft());
        const auto rightResult = AnalyzeExpression(e->GetRight());
        return leftResult && rightResult;
    }

    if (leftType != rightType)
    {
        PrintErrorPrefix(std::cerr, expression.get())
            << "Invalid operands, can't apply \""
            << e->GetOperator() << "\" to operands of types \""
            << leftType << "\" and \"" << rightType << "\""
            << std::endl;
        return false;
    }

    if (e->HasArithmeticOperator() || !e->HasAnyOfOperators({BinaryExpression::OP_EQ, BinaryExpression::OP_NEQ}))
    {
        if (leftType == Types::Int || leftType == Types::UsignedInt)
            return true;

        PrintErrorPrefix(std::cerr, expression.get())
            << "Invalid operands, can't apply \""
            << e->GetOperator() << "\" to operands of type \""
            << leftType << "\""
            << std::endl;
        return false;
    }

    return true;
}

std::string SemanticAnalyzer::ResolveExpressionType(const std::unique_ptr<const Expression> &expression)
{
    switch (expression->GetType())
    {
    case AstNodeType::IdExpression:
        return GetIdExpressionType(expression);
    case AstNodeType::NumLiteralExpression:
        return GetNumLiteralExpressionType(expression);
    case AstNodeType::BoolLiteralExpression:
        return Types::Bool;
    case AstNodeType::BinaryExpression:
        return ResolveBinaryExpressionType(expression);
    case AstNodeType::FunctionCallExpression:
        return GetFunctionCallExpressionType(expression);
    default:
        PrintErrorPrefix(std::cerr, expression.get()) << "Can't resolve type for given expression" << std::endl;
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
    if (rightType == Types::Unknown || rightType != leftType)
        return Types::Unknown;

    if (binaryExpression->HasArithmeticOperator())
        return leftType;

    return Types::Bool;
}

std::ostream &SemanticAnalyzer::PrintErrorPrefix(std::ostream &out, const AstNode *node) const
{
    const auto location = node->GetLocation();
    out << "[SemanticAnalyzer] ERROR: line " << location.line << " column " << location.column << ": ";
    return out;
}