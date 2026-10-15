#include "ast/expressions/id_expression.hpp"
#include "runtime/bindings/variable_binding.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

const std::string &IdExpression::GetValue() const
{
    return value;
}

void IdExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("IdExpression");
    printer.Field("Value", value);
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> IdExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    auto environment = interpreter.GetCurrentEnvironment();
    if (!environment->BindingExists(value, BindingKind::Variable))
    {
        std::cerr << "[IdExpression] ERROR: Lookup failed for variable " << value << std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto &binding = environment->GetBinding(value, BindingKind::Variable);
    auto variable = dynamic_cast<VariableBinding *>(binding.get());
    return variable->GetValue()->Clone();
}
