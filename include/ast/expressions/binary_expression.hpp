#ifndef BINARY_EXPRESSION_HPP_INCLUDED
#define BINARY_EXPRESSION_HPP_INCLUDED

#include "ast/expressions/expression.hpp"
#include <map>

class BinaryExpression : public Expression
{
private:
    std::unique_ptr<Expression> left, right;
    std::string op;

public:
    BinaryExpression(SourceLocation location,
                     std::string op,
                     std::unique_ptr<Expression> left,
                     std::unique_ptr<Expression> right)
        : Expression(AstNodeType::BinaryExpression, location),
          left(std::move(left)),
          right(std::move(right)),
          op(std::move(op)) {}

    const std::unique_ptr<Expression> &GetLeft() const;
    const std::unique_ptr<Expression> &GetRight() const;
    const std::string &GetOperator() const;
    bool HasArithmeticOperator() const;
    bool HasAnyOfOperators(std::initializer_list<std::string_view> ops) const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;

    // Arithmetic operators:
    constexpr static const std::string_view Plus = "+";
    constexpr static const std::string_view Minus = "-";
    constexpr static const std::string_view Mul = "*";
    constexpr static const std::string_view Div = "/";

    // Relational operators:
    constexpr static const std::string_view Eq = "==";
    constexpr static const std::string_view Neq = "!=";
    constexpr static const std::string_view Lt = "<";
    constexpr static const std::string_view Lte = "<=";
    constexpr static const std::string_view Gt = ">";
    constexpr static const std::string_view Gte = ">=";

    // Logical operators:
    constexpr static const std::string_view And = "&&";
    constexpr static const std::string_view OP_OR = "||";
};
#endif
