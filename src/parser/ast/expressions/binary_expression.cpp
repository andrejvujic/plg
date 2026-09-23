#include "binary_expression.hpp"

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

std::string_view BinaryExpression::GetOperator() const
{
    return op;
}

void BinaryExpression::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');

    out << indent << "[BinaryExpression] {\n";
    left->Print(out, depth + 1);
    out << fieldIndent << "Operator -> " << op << "\n";
    right->Print(out, depth + 1);
    out << indent << "}\n";
}

std::string_view BinaryExpression::OP_PLUS = "+";
std::string_view BinaryExpression::OP_MINUS = "-";
std::string_view BinaryExpression::OP_MUL = "*";
std::string_view BinaryExpression::OP_DIV = "/";