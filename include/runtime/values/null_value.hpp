#ifndef NULL_VALUE_HPP_INCLUDED
#define NULL_VALUE_HPP_INCLUDED

#include "runtime/values/runtime_value.hpp"

class NullValue : public RuntimeValue
{
public:
    NullValue() : RuntimeValue(RuntimeValueType::Null) {}

    std::unique_ptr<RuntimeValue> Clone() const override;
    std::ostream &Print(std::ostream &out) const override;
};

#endif
