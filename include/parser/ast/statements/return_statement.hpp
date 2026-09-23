#ifndef RETURN_STATEMENT_HPP_INCLUDED
#define RETURN_STATEMENT_HPP_INCLUDED

#include "parser/ast/statements/statement.hpp"

class ReturnStatement : public Statement
{
private:
public:
    ReturnStatement();

    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
