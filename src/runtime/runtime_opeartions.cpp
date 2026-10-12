#include "runtime/runtime_operations.hpp"
#include "runtime/null_value.hpp"
#include "runtime/int_value.hpp"
#include "runtime/unsigned_int_value.hpp"

std::unique_ptr<RuntimeValue> RuntimeOperations::Add(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        return std::make_unique<NullValue>();

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<IntValue>(aa->GetValue() + bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<UnsignedIntValue>(aa->GetValue() + bb->GetValue());
    }

    return std::make_unique<NullValue>();
}