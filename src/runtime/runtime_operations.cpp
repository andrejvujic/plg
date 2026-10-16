#include "runtime/runtime_operations.hpp"
#include "runtime/values/null_value.hpp"
#include "runtime/values/int_value.hpp"
#include "runtime/values/unsigned_int_value.hpp"
#include "runtime/values/bool_value.hpp"

std::unique_ptr<RuntimeValue> RuntimeOperations::Add(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

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

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::Sub(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<IntValue>(aa->GetValue() - bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<UnsignedIntValue>(aa->GetValue() - bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::Mul(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<IntValue>(aa->GetValue() * bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<UnsignedIntValue>(aa->GetValue() * bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::Div(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        if (!bb->GetValue())
        {
            std::cerr << "[RuntimeOperations] ERROR: Division by zero" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return std::make_unique<IntValue>(aa->GetValue() / bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());

        if (!bb->GetValue())
        {
            std::cerr << "[RuntimeOperations] ERROR: Division by zero" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return std::make_unique<UnsignedIntValue>(aa->GetValue() / bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::Eq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() == bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() == bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::Bool)
    {
        auto aa = dynamic_cast<BoolValue *>(a.get());
        auto bb = dynamic_cast<BoolValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() == bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::NotEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() != bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() != bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::Bool)
    {
        auto aa = dynamic_cast<BoolValue *>(a.get());
        auto bb = dynamic_cast<BoolValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() != bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::LessThan(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() < bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() < bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::LessThenOrEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() <= bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() <= bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::GreaterThan(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() > bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() > bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::GreaterThenOrEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Int)
    {
        auto aa = dynamic_cast<IntValue *>(a.get());
        auto bb = dynamic_cast<IntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() >= bb->GetValue());
    }

    if (a->GetType() == RuntimeValueType::UnsignedInt)
    {
        auto aa = dynamic_cast<UnsignedIntValue *>(a.get());
        auto bb = dynamic_cast<UnsignedIntValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() >= bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::And(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Bool)
    {
        auto aa = dynamic_cast<BoolValue *>(a.get());
        auto bb = dynamic_cast<BoolValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() && bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> RuntimeOperations::Or(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b)
{
    if (!a || !b || a->GetType() != b->GetType())
        std::exit(EXIT_FAILURE);

    if (a->GetType() == RuntimeValueType::Bool)
    {
        auto aa = dynamic_cast<BoolValue *>(a.get());
        auto bb = dynamic_cast<BoolValue *>(b.get());
        return std::make_unique<BoolValue>(aa->GetValue() || bb->GetValue());
    }

    std::exit(EXIT_FAILURE);
}
