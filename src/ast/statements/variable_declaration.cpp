#include "ast/statements/variable_declaration.hpp"

#include "runtime/bindings/variable_binding.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/values/null_value.hpp"

const std::string &VariableDeclaration::GetVarName() const
{
    return varName;
}

const std::string &VariableDeclaration::GetVarType() const
{
    return varType;
}

const std::unique_ptr<Expression> &VariableDeclaration::GetInitialValue() const
{
    return initialValue;
}

bool VariableDeclaration::HasInitialValue() const
{
    return initialValue != nullptr;
}

void VariableDeclaration::Print(AstPrinter &printer) const
{
    printer.BeginNode("VariableDeclaration");
    printer.Field("Var. Name", varName);
    printer.Field("Var. Type", varType);
    printer.Field("Initial Value", initialValue.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> VariableDeclaration::Execute(TreeWalkInterpreter &interpreter) const
{
    auto varValue = initialValue ? initialValue->Execute(interpreter) : std::make_unique<NullValue>();
    auto varValueClone = varValue->Clone();

    auto environment = interpreter.GetCurrentEnvironment();
    std::unique_ptr<Binding> variable = std::make_unique<VariableBinding>(
        varName,
        varType,
        std::move(varValue));
    environment->DeclareBinding(std::move(variable));

    return varValueClone;
}
