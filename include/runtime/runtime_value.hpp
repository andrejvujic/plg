#ifndef RUNTIME_VALUE_HPP_INCLUDED
#define RUNTIME_VALUE_HPP_INCLUDED

#include <iostream>

enum class RuntimeValueType
{
    Null,
    Int,
    UnsignedInt,
    Bool
};

class RuntimeValue
{
private:
    RuntimeValueType type;

public:
    RuntimeValue(RuntimeValueType type) : type(type) {}
    virtual ~RuntimeValue() = default;

    RuntimeValueType GetType() const;

    virtual std::unique_ptr<RuntimeValue> Clone() const = 0;
    virtual std::ostream &Print(std::ostream &out) const = 0;
};

#endif