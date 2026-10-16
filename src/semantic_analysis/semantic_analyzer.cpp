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
#include "ast/expressions/call_expression.hpp"
#include "ast/expressions/assignment_expression.hpp"
#include "ast/expressions/binary_expression.hpp"

#include <iostream>
#include <cmath>

void SemanticAnalyzer::AnalyzeProgram(const std::unique_ptr<Program> &program)
{
    auto result = true;
    for (auto &statement : program->GetStatements())
        result = AnalyzeStatement(statement) && result;

    if (!result)
        std::exit(EXIT_FAILURE);
}

bool SemanticAnalyzer::AnalyzeFunctionDeclaration(const std::unique_ptr<Statement> &statement)
{
    auto functionDeclaration = dynamic_cast<const FunctionDeclaration *>(statement.get());

    std::unique_ptr<Symbol> symbol = std::make_unique<FunctionSymbol>(
        functionDeclaration->GetName(),
        functionDeclaration->GetReturnType(),
        *functionDeclaration);
    currentSymbolTable->DeclareSymbol(std::move(symbol));

    auto functionSymbolTable = std::make_shared<SymbolTable>(currentSymbolTable);
    currentSymbolTable = functionSymbolTable;
    for (auto &parameter : functionDeclaration->GetParameters())
    {
        if (!currentSymbolTable->SymbolExists(parameter->GetVarName(), SymbolKind::Variable, SymbolTable::Local))
        {
            std::unique_ptr<Symbol> symbol = std::make_unique<VariableSymbol>(
                parameter->GetVarName(),
                parameter->GetVarType());
            currentSymbolTable->DeclareSymbol(std::move(symbol));
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
        result = AnalyzeStatement(statement) && result;
    functionStack.pop();

    currentSymbolTable = currentSymbolTable->GetParent();

    return result;
    // TODO: Check if a value is returned from non-void functions
}

bool SemanticAnalyzer::AnalyzeStatement(const std::unique_ptr<Statement> &statement)
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

bool SemanticAnalyzer::AnalyzeComplexStatement(const std::unique_ptr<Statement> &statement)
{
    auto result = true;
    auto complexStatement = dynamic_cast<const ComplexStatement *>(statement.get());

    auto bodySymbolTable = std::make_shared<SymbolTable>(currentSymbolTable);
    currentSymbolTable = bodySymbolTable;
    for (auto &childStatement : complexStatement->GetStatements())
        result = AnalyzeStatement(childStatement) && result;

    currentSymbolTable = bodySymbolTable->GetParent();
    return result;
}

bool SemanticAnalyzer::AnalyzeIfStatement(const std::unique_ptr<Statement> &statement)
{
    auto ifStatement = dynamic_cast<const IfStatement *>(statement.get());

    auto result = AnalyzeExpression(ifStatement->GetCondition());
    auto conditionType = ResolveExpressionType(ifStatement->GetCondition());
    if (conditionType != Types::Bool)
    {
        result = false;
        PrintErrorPrefix(std::cerr, ifStatement)
            << "Got invalid type after resolving condition expression, expected \"bool\" but got \"" << conditionType << "\""
            << std::endl;
    }

    auto consequentSymbolTable = std::make_shared<SymbolTable>(currentSymbolTable);
    currentSymbolTable = consequentSymbolTable;
    result = AnalyzeStatement(ifStatement->GetConsequentStatement()) && result;

    currentSymbolTable = consequentSymbolTable->GetParent();

    if (!ifStatement->HasAlternateStatement())
        return result;

    auto alternateSymbolTable = std::make_shared<SymbolTable>(currentSymbolTable);
    currentSymbolTable = alternateSymbolTable;
    result = AnalyzeStatement(ifStatement->GetAlternateStatement()) && result;

    currentSymbolTable = alternateSymbolTable->GetParent();

    return result;
}

bool SemanticAnalyzer::AnalyzeForStatement(const std::unique_ptr<Statement> &statement)
{
    auto forStatement = dynamic_cast<const ForStatement *>(statement.get());

    auto result = AnalyzeExpression(forStatement->GetInitExpression()); // TODO: This needs to be an AssignmentExpression.

    result = AnalyzeExpression(forStatement->GetCondition()) && result;
    auto conditionType = ResolveExpressionType(forStatement->GetCondition());
    if (conditionType != Types::Bool)
    {
        result = false;
        PrintErrorPrefix(std::cerr, forStatement)
            << "Got invalid type after resolving condition expression, expected \"bool\" but got \"" << conditionType << "\""
            << std::endl;
    }

    result = AnalyzeExpression(forStatement->GetUpdateExpression()) && result; // TODO: This needs to be an AssignmentExpression.

    auto bodySymbolTable = std::make_shared<SymbolTable>(currentSymbolTable);
    currentSymbolTable = bodySymbolTable;
    result = AnalyzeStatement(forStatement->GetBody()) && result;

    currentSymbolTable = bodySymbolTable->GetParent();
    return result;
}

bool SemanticAnalyzer::AnalyzeExpressionStatement(const std::unique_ptr<Statement> &statement)
{
    auto expressionStatement = dynamic_cast<const ExpressionStatement *>(statement.get());
    return AnalyzeExpression(expressionStatement->GetExpression());
}

bool SemanticAnalyzer::AnalyzeReturnStatement(const std::unique_ptr<Statement> &statement)
{
    auto returnStatement = dynamic_cast<const ReturnStatement *>(statement.get());
    if (functionStack.empty() || !currentSymbolTable->SymbolExists(functionStack.top(), SymbolKind::Function))
    {
        PrintErrorPrefix(std::cerr, returnStatement)
            << "Unexpected return statement"
            << std::endl;
        return false;
    }

    auto &functionSymbol = currentSymbolTable->GetSymbol(functionStack.top(), SymbolKind::Function);

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

    const auto isExpressionValid = AnalyzeExpression(returnStatement->GetValue());
    const auto returnType = ResolveExpressionType(returnStatement->GetValue());
    if (returnType == functionSymbol->GetType())
        return isExpressionValid;

    PrintErrorPrefix(std::cerr, returnStatement)
        << "Invalid return type, expected \""
        << functionSymbol->GetType() << "\" but got \""
        << returnType << "\""
        << std::endl;

    return false;
}

bool SemanticAnalyzer::AnalyzeVariableDeclaration(const std::unique_ptr<Statement> &statement)
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

    if (currentSymbolTable->SymbolExists(variableDeclaration->GetVarName(), SymbolKind::Variable, SymbolTable::Local))
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
    currentSymbolTable->DeclareSymbol(std::move(symbol));

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

bool SemanticAnalyzer::AnalyzeExpression(const std::unique_ptr<Expression> &expression)
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
    case AstNodeType::CallExpression:
        return AnalyzeCallExpression(expression);
    case AstNodeType::AssignmentExpression:
        return AnalyzeAssignmentExpression(expression);
    default:
        PrintErrorPrefix(std::cerr, expression.get()) << "Semantic analysis not implemented for given expression" << std::endl;
        return false;
    }
}

bool SemanticAnalyzer::AnalyzeIdExpression(const std::unique_ptr<Expression> &expression)
{
    const IdExpression *e = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentSymbolTable->SymbolExists(e->GetValue(), SymbolKind::Variable))
    {
        PrintErrorPrefix(std::cerr, e)
            << "Variable \"" << e->GetValue()
            << "\" doesn't exist"
            << std::endl;
        return false;
    }

    return true;
}

bool SemanticAnalyzer::AnalyzeCallExpression(const std::unique_ptr<Expression> &expression)
{
    const CallExpression *call = dynamic_cast<const CallExpression *>(expression.get());
    auto &callee = call->GetCallee();

    if (callee->GetType() == AstNodeType::IdExpression)
        return AnalyzeNonMemberFunctionCall(call);

    std::cerr << "[SemanticAnalyzer] Semantic analysis not implemented for " << callee->GetType() << std::endl;
    exit(EXIT_FAILURE);
}

bool SemanticAnalyzer::AnalyzeNonMemberFunctionCall(const CallExpression *call)
{
    auto &callee = call->GetCallee();
    auto id = dynamic_cast<IdExpression *>(callee.get());

    if (!currentSymbolTable->SymbolExists(id->GetValue(), SymbolKind::Function))
    {
        PrintErrorPrefix(std::cerr, call)
            << "Function \"" << call->GetCallee()
            << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &symbol = currentSymbolTable->GetSymbol(id->GetValue(), SymbolKind::Function);
    auto function = dynamic_cast<const FunctionSymbol *>(symbol.get());

    if (function->GetParameterCount() != call->GetArgumentCount())
    {
        PrintErrorPrefix(std::cerr, call)
            << "Invalid number of arguments in function call, expected "
            << function->GetParameterCount() << " but got " << call->GetArgumentCount()
            << std::endl;
        return false;
    }
    return AnalyzeCallArguments(call->GetArguments(), function->GetParameterTypes());
}

bool SemanticAnalyzer::AnalyzeCallArguments(const std::vector<std::unique_ptr<Expression>> &arguments, const std::vector<std::string> &parameterTypes)
{
    if (arguments.size() != parameterTypes.size())
        return false;

    auto result = true;
    for (size_t index = 0; index < arguments.size(); ++index)
    {
        if (!AnalyzeExpression(arguments[index]))
        {
            result = false;
            continue;
        }

        auto argumentType = ResolveExpressionType(arguments[index]);
        if (argumentType != parameterTypes[index])
        {
            PrintErrorPrefix(std::cerr, arguments[index].get())
                << "Positional argument at position " << index
                << " is of wrong type, expected \"" << parameterTypes[index] << "\" but got \"" << argumentType << "\""
                << std::endl;
            result = false;
        }
    }

    return result;
}

bool SemanticAnalyzer::AnalyzeAssignmentExpression(const std::unique_ptr<Expression> &expression)
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
    if (!currentSymbolTable->SymbolExists(assigneeIdExpression->GetValue(), SymbolKind::Variable))
    {
        PrintErrorPrefix(std::cerr, assignmentExpression)
            << "Variable \""
            << assigneeIdExpression->GetValue() << "\" doesn't exist"
            << std::endl;
        return false;
    }

    auto &symbol = currentSymbolTable->GetSymbol(assigneeIdExpression->GetValue(), SymbolKind::Variable);
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

bool SemanticAnalyzer::AnalyzeBinaryExpression(const std::unique_ptr<Expression> &expression)
{
    const BinaryExpression *e = dynamic_cast<const BinaryExpression *>(expression.get());

    const auto leftResult = AnalyzeExpression(e->GetLeft());
    const auto rightResult = AnalyzeExpression(e->GetRight());
    if (!leftResult || !rightResult)
        return false;

    const auto leftType = ResolveExpressionType(e->GetLeft());
    const auto rightType = ResolveExpressionType(e->GetRight());

    if (leftType == Types::Unknown || rightType == Types::Unknown)
        return false;

    if (leftType != rightType)
    {
        PrintErrorPrefix(std::cerr, expression.get())
            << "Invalid operands, can't apply \""
            << e->GetOperator() << "\" to operands of types \""
            << leftType << "\" and \"" << rightType << "\""
            << std::endl;
        return false;
    }

    if (e->HasAnyOfOperators({BinaryExpression::And, BinaryExpression::OP_OR}))
        return leftType == Types::Bool;

    if (e->HasArithmeticOperator() || !e->HasAnyOfOperators({BinaryExpression::Eq, BinaryExpression::Neq}))
    {
        if (leftType != Types::Int && leftType != Types::UsignedInt)
            PrintErrorPrefix(std::cerr, expression.get())
                << "Invalid operands, can't apply \""
                << e->GetOperator() << "\" to operands of type \""
                << leftType << "\""
                << std::endl;

        return leftType == Types::Int || leftType == Types::UsignedInt;
    }

    return true;
}

std::string SemanticAnalyzer::ResolveExpressionType(const std::unique_ptr<Expression> &expression)
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
    case AstNodeType::CallExpression:
        return GetCallExpressionType(expression);
    default:
        PrintErrorPrefix(std::cerr, expression.get()) << "Can't resolve type for given expression" << std::endl;
        return Types::Unknown;
    }
}

std::string SemanticAnalyzer::GetIdExpressionType(const std::unique_ptr<Expression> &expression)
{
    auto idExpression = dynamic_cast<const IdExpression *>(expression.get());
    if (!currentSymbolTable->SymbolExists(idExpression->GetValue(), SymbolKind::Variable))
        return Types::Unknown;

    auto &symbol = currentSymbolTable->GetSymbol(idExpression->GetValue(), SymbolKind::Variable);
    return symbol->GetType();
}

std::string SemanticAnalyzer::GetNumLiteralExpressionType(const std::unique_ptr<Expression> &expression)
{
    auto e = dynamic_cast<const NumLiteralExpression *>(expression.get());
    return e->GetLiteralType();
}

std::string SemanticAnalyzer::GetCallExpressionType(const std::unique_ptr<Expression> &expression)
{
    auto callExpression = dynamic_cast<const CallExpression *>(expression.get());

    auto &callee = callExpression->GetCallee();
    if (callee->GetType() == AstNodeType::IdExpression)
    {
        auto id = dynamic_cast<IdExpression *>(callee.get());
        if (!currentSymbolTable->SymbolExists(id->GetValue(), SymbolKind::Function))
            return Types::Unknown;

        auto &symbol = currentSymbolTable->GetSymbol(id->GetValue(), SymbolKind::Function);
        return symbol->GetType();
    }

    std::cerr << "[SemanticAnalysis] ERROR Couldn't resolve type of unsupported node type: " << callee->GetType() << std::endl;
    std::exit(EXIT_FAILURE);
}

std::string SemanticAnalyzer::ResolveBinaryExpressionType(const std::unique_ptr<Expression> &expression)
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
