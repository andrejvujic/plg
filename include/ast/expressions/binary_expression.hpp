#ifndef BINARY_EXPRESSION_HPP_INCLUDED
#define BINARY_EXPRESSION_HPP_INCLUDED

#include "ast/expressions/expression.hpp"

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
    const std::string &GetOperator() const;

    void Print(AstPrinter &printer) const override;

    static std::string_view OP_PLUS, OP_MINUS, OP_MUL, OP_DIV;
};
#endif
