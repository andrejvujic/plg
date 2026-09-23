#ifndef BINARY_EXPRESSION_HPP_INCLUDED
#define BINARY_EXPRESSION_HPP_INCLUDED

#include "parser/ast/expressions/expression.hpp"

class BinaryExpression : public Expression
{
private:
    std::unique_ptr<const Expression> left, right;
    std::string op;

public:
    BinaryExpression();

    void SetOperator(std::string op);
    void SetLeft(std::unique_ptr<const Expression> &left);
    void SetRight(std::unique_ptr<const Expression> &right);

    const std::unique_ptr<const Expression> &GetLeft() const;
    const std::unique_ptr<const Expression> &GetRight() const;
    std::string_view GetOperator() const;

    void Print(std::ostream &out, size_t depth = 0) const override;

    static std::string_view OP_PLUS, OP_MINUS, OP_MUL, OP_DIV;
};
#endif
