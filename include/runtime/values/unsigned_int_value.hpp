#ifndef UNSIGNED_INT_VALUE_HPP_INCLUDED
#define UNSIGNED_INT_VALUE_HPP_INCLUDED

#include "runtime/values/runtime_value.hpp"

class UnsignedIntValue : public RuntimeValue
{
private:
    unsigned int value;

public:
    UnsignedIntValue(unsigned int value) : RuntimeValue(RuntimeValueType::UnsignedInt), value(value) {}

    unsigned int GetValue() const;
    std::unique_ptr<RuntimeValue> Clone() const override;

    std::ostream &Print(std::ostream &out) const override;
};

#endif
