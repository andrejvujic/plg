#ifndef INT_VALUE_HPP_INCLUDED
#define INT_VALUE_HPP_INCLUDED

#include "runtime/values/runtime_value.hpp"

class IntValue : public RuntimeValue
{
private:
    int value;

public:
    IntValue(int value) : RuntimeValue(RuntimeValueType::Int), value(value) {}

    int GetValue() const;
    std::unique_ptr<RuntimeValue> Clone() const override;

    std::ostream &Print(std::ostream &out) const override;
};

#endif
