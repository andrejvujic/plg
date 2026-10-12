#ifndef BOOL_VALUE_HPP_INCLUDED
#define BOOL_VALUE_HPP_INCLUDED

#include "runtime/runtime_value.hpp"

class BoolValue : public RuntimeValue
{
private:
    bool value;

public:
    BoolValue(bool value) : RuntimeValue(RuntimeValueType::Bool), value(value) {}

    bool GetValue() const;
    std::unique_ptr<RuntimeValue> Clone() const override;

    std::ostream &Print(std::ostream &out) const override;
};

#endif