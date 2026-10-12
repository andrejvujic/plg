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
    bool HasArithmeticOperator() const;
    bool HasAnyOfOperators(std::initializer_list<std::string_view> operators) const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;

    // Arithmetic operators:
    constexpr static std::string_view OP_PLUS = "+";
    constexpr static std::string_view OP_MINUS = "-";
    constexpr static std::string_view OP_MUL = "*";
    constexpr static std::string_view OP_DIV = "/";

    // Relational operators:
    constexpr static const std::string_view OP_EQ = "==";
    constexpr static const std::string_view OP_NEQ = "!=";
    constexpr static const std::string_view OP_LT = "<";
    constexpr static const std::string_view OP_LTE = "<=";
    constexpr static const std::string_view OP_GT = ">";
    constexpr static const std::string_view OP_GTE = ">=";
};
#endif
