#include "ast/expressions/binary_expression.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/runtime_operations.hpp"

const std::unique_ptr<Expression> &BinaryExpression::GetLeft() const
{
    return left;
};

const std::unique_ptr<Expression> &BinaryExpression::GetRight() const
{
    return right;
};

const std::string &BinaryExpression::GetOperator() const
{
    return op;
}

bool BinaryExpression::HasArithmeticOperator() const
{
    return HasAnyOfOperators({BinaryExpression::Plus,
                              BinaryExpression::Minus,
                              BinaryExpression::Mul,
                              BinaryExpression::Div});
}

bool BinaryExpression::HasAnyOfOperators(std::initializer_list<std::string_view> operators) const
{
    return std::any_of(
        operators.begin(),
        operators.end(),
        [this](auto op)
        { return this->op == op; });
}

std::unique_ptr<RuntimeValue> BinaryExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    auto leftValue = left->Execute(interpreter);
    auto rightValue = right->Execute(interpreter);

    if (op == BinaryExpression::Plus)
        return RuntimeOperations::Add(leftValue, rightValue);

    if (op == BinaryExpression::Minus)
        return RuntimeOperations::Sub(leftValue, rightValue);

    if (op == BinaryExpression::Mul)
        return RuntimeOperations::Mul(leftValue, rightValue);

    if (op == BinaryExpression::Div)
        return RuntimeOperations::Div(leftValue, rightValue);

    if (op == BinaryExpression::Eq)
        return RuntimeOperations::Eq(leftValue, rightValue);

    if (op == BinaryExpression::Neq)
        return RuntimeOperations::NotEq(leftValue, rightValue);

    if (op == BinaryExpression::Lt)
        return RuntimeOperations::LessThan(leftValue, rightValue);

    if (op == BinaryExpression::Lte)
        return RuntimeOperations::LessThenOrEq(leftValue, rightValue);

    if (op == BinaryExpression::Gt)
        return RuntimeOperations::GreaterThan(leftValue, rightValue);

    if (op == BinaryExpression::Gte)
        return RuntimeOperations::GreaterThenOrEq(leftValue, rightValue);

    if (op == BinaryExpression::And)
        return RuntimeOperations::And(leftValue, rightValue);

    if (op == BinaryExpression::OP_OR)
        return RuntimeOperations::Or(leftValue, rightValue);

    std::cerr << "[BinaryExpression] ERROR: Execution not implemented for operation \"" << op << "\"" << std::endl;
    std::exit(EXIT_FAILURE);
}

void BinaryExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("BinaryExpression");
    printer.Field("Operator", op);
    printer.Field("Left", left.get());
    printer.Field("Right", right.get());
    printer.EndNode();
}
