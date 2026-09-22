#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include <vector>

#include "ast.hpp"
#include "functions/functions.hpp"

class Program : public AstNode
{
private:
    std::vector<std::unique_ptr<const Function>> body;

public:
    Program();

    void AddFunction(std::unique_ptr<const Function> &function);
    const std::vector<std::unique_ptr<const Function>> &GetFunctions() const;
    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
