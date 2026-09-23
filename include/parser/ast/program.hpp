#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include <vector>

#include "parser/ast/ast.hpp"
#include "parser/ast/function.hpp"

class Program : public AstNode
{
private:
    std::vector<std::unique_ptr<const Function>> body;

public:
    Program();

    void AddFunction(std::unique_ptr<const Function> &function);
    const std::vector<std::unique_ptr<const Function>> &GetFunctions() const;
    void Print(AstPrinter &printer) const override;
};

#endif
