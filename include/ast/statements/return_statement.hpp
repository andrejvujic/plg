#ifndef RETURN_STATEMENT_HPP_INCLUDED
#define RETURN_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class ReturnStatement : public Statement
{
private:
    std::unique_ptr<Expression> value;

public:
    ReturnStatement();

    bool HasValue() const;
    const std::unique_ptr<Expression> &GetValue() const;
    void SetValue(std::unique_ptr<Expression> &value);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
