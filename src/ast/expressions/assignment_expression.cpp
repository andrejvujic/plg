#include "ast/expressions/assignment_expression.hpp"
#include "ast/expressions/id_expression.hpp"
#include "semantic_analysis/symbols/variable_symbol.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

AssignmentExpression::AssignmentExpression() : Expression(AstNodeType::AssignmentExpression)
{
}

const std::unique_ptr<const Expression> &AssignmentExpression::GetAssignee() const
{
    return assignee;
}

const std::unique_ptr<const Expression> &AssignmentExpression::GetValue() const
{
    return value;
}

void AssignmentExpression::SetAssignee(std::unique_ptr<const Expression> &assignee)
{
    this->assignee = std::move(assignee);
}

void AssignmentExpression::SetValue(std::unique_ptr<const Expression> &value)
{
    this->value = std::move(value);
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
        auto scope = interpreter.GetCurrentScope();

        if (!scope->SymbolExists(id->GetValue(), SymbolKind::Variable))
            std::exit(EXIT_FAILURE);

        auto &symbol = scope->GetSymbol(id->GetValue(), SymbolKind::Variable);
        auto varSymbol = dynamic_cast<VariableSymbol *>(symbol.get());

        auto varValue = value->Execute(interpreter);
        auto varValueClone = varValue->Clone();
        varSymbol->SetValue(varValue);

        return varValueClone;
    }

    std::cerr << "[AssignmentExpression] ERROR: Execution not implemented for given type" << std::endl;
    std::exit(EXIT_FAILURE);
}