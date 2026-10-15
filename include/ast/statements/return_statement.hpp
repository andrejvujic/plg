#ifndef RETURN_STATEMENT_HPP_INCLUDED
#define RETURN_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class ReturnStatement : public Statement
{
private:
    std::unique_ptr<Expression> value;

public:
    explicit ReturnStatement(SourceLocation location)
        : Statement(AstNodeType::ReturnStatement, location) {}

    ReturnStatement(SourceLocation location, std::unique_ptr<Expression> value)
        : Statement(AstNodeType::ReturnStatement, location), value(std::move(value)) {}

    bool HasValue() const;
    const std::unique_ptr<Expression> &GetValue() const;
    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
