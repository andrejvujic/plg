#ifndef RETURN_STATEMENT_HPP_INCLUDED
#define RETURN_STATEMENT_HPP_INCLUDED

#include "parser/ast/statements/statement.hpp"

class ReturnStatement : public Statement
{
private:
    std::unique_ptr<const Expression> value;

public:
    ReturnStatement();

    bool HasValue() const;
    const std::unique_ptr<const Expression> &GetValue() const;
    void SetValue(std::unique_ptr<const Expression> &value);

    void Print(AstPrinter &printer) const override;
};

#endif
