#ifndef VARIABLE_SYMBOL_HPP_INCLUDED
#define VARIABLE_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"

class VariableSymbol : public Symbol
{
private:
public:
    VariableSymbol(std::string name, std::string type);
};

#endif