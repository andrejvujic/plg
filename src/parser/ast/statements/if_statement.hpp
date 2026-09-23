#ifndef IF_STATEMENT_HPP_INCLUDED
#define IF_STATEMENT_HPP_INCLUDED

#include "statement.hpp"

class IfStatement : public Statement
{
private:
public:
    IfStatement();

    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif