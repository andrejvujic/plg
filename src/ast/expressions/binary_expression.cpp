#include "ast/expressions/binary_expression.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/runtime_operations.hpp"

BinaryExpression::BinaryExpression() : Expression(AstNodeType::BinaryExpression)
{
}

void BinaryExpression::SetLeft(std::unique_ptr<const Expression> &left)
{
    this->left = std::move(left);
}

void BinaryExpression::SetRight(std::unique_ptr<const Expression> &right)
{
    this->right = std::move(right);
}

void BinaryExpression::SetOperator(std::string op)
{
    this->op = op;
}

const std::unique_ptr<const Expression> &BinaryExpression::GetLeft() const
{
    return left;
};

const std::unique_ptr<const Expression> &BinaryExpression::GetRight() const
{
    return right;
};

const std::string &BinaryExpression::GetOperator() const
{
    return op;
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

    std::cerr << "[BinaryExpression] ERROR: Execution unsupported for operation \"" << op << "\"" << std::endl;
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
