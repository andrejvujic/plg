#include "ast/expressions/binary_expression.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/runtime_operations.hpp"

BinaryExpression::BinaryExpression() : Expression(AstNodeType::BinaryExpression)
{
}

void BinaryExpression::SetLeft(std::unique_ptr<Expression> &left)
{
    this->left = std::move(left);
}

void BinaryExpression::SetRight(std::unique_ptr<Expression> &right)
{
    this->right = std::move(right);
}

void BinaryExpression::SetOperator(std::string op)
{
    this->op = op;
}

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
    return HasAnyOfOperators({BinaryExpression::OP_PLUS,
                              BinaryExpression::OP_MINUS,
                              BinaryExpression::OP_MUL,
                              BinaryExpression::OP_DIV});
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

    if (op == BinaryExpression::OP_PLUS)
        return RuntimeOperations::Add(leftValue, rightValue);

    if (op == BinaryExpression::OP_MINUS)
        return RuntimeOperations::Sub(leftValue, rightValue);

    if (op == BinaryExpression::OP_MUL)
        return RuntimeOperations::Mul(leftValue, rightValue);

    if (op == BinaryExpression::OP_DIV)
        return RuntimeOperations::Div(leftValue, rightValue);

    if (op == BinaryExpression::OP_EQ)
        return RuntimeOperations::Eq(leftValue, rightValue);

    if (op == BinaryExpression::OP_NEQ)
        return RuntimeOperations::NotEq(leftValue, rightValue);

    if (op == BinaryExpression::OP_LT)
        return RuntimeOperations::LessThan(leftValue, rightValue);

    if (op == BinaryExpression::OP_LTE)
        return RuntimeOperations::LessThenOrEq(leftValue, rightValue);

    if (op == BinaryExpression::OP_GT)
        return RuntimeOperations::GreaterThan(leftValue, rightValue);

    if (op == BinaryExpression::OP_GTE)
        return RuntimeOperations::GreaterThenOrEq(leftValue, rightValue);

    if (op == BinaryExpression::OP_AND)
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
