#include "ast/expressions/assignment_expression.hpp"
#include "ast/expressions/id_expression.hpp"
#include "runtime/bindings/variable_binding.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

const std::unique_ptr<Expression> &AssignmentExpression::GetAssignee() const
{
    return assignee;
}

const std::unique_ptr<Expression> &AssignmentExpression::GetValue() const
{
    return value;
}

void AssignmentExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("AssignmentExpression");
    printer.Field("Assignee", assignee.get());
    printer.Field("Value", value.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> AssignmentExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    if (assignee->GetType() == AstNodeType::IdExpression)
    {
        auto id = dynamic_cast<const IdExpression *>(assignee.get());
        auto environment = interpreter.GetCurrentEnvironment();

        if (!environment->BindingExists(id->GetValue(), BindingKind::Variable))
            std::exit(EXIT_FAILURE);

        auto &binding = environment->GetBinding(id->GetValue(), BindingKind::Variable);
        auto variable = dynamic_cast<VariableBinding *>(binding.get());

        auto varValue = value->Execute(interpreter);
        auto varValueClone = varValue->Clone();
        variable->SetValue(std::move(varValue));

        return varValueClone;
    }

    std::cerr << "[AssignmentExpression] ERROR: Execution not implemented for given type" << std::endl;
    std::exit(EXIT_FAILURE);
}
