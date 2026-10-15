#include "ast/statements/if_statement.hpp"

#include "runtime/runtime_value.hpp"
#include "runtime/bool_value.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "semantic_analysis/scoped_new_scope.hpp"

IfStatement::IfStatement() : Statement(AstNodeType::IfStatement)
{
}

const std::unique_ptr<const Expression> &IfStatement::GetCondition() const
{
    return condition;
}

const std::unique_ptr<const Statement> &IfStatement::GetConsequentStatement() const
{
    return consequent;
}

const std::unique_ptr<const Statement> &IfStatement::GetAlternateStatement() const
{
    return alternate;
}

bool IfStatement::HasAlternateStatement() const
{
    return alternate != nullptr;
}

void IfStatement::SetConsequentStatement(std::unique_ptr<const Statement> &statement)
{
    this->consequent = std::move(statement);
}

void IfStatement::SetAlternateStatement(std::unique_ptr<const Statement> &statement)
{
    this->alternate = std::move(statement);
}

void IfStatement::SetCondition(std::unique_ptr<const Expression> &condition)
{
    this->condition = std::move(condition);
}

void IfStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("IfStatement");
    printer.Field("Condition", condition.get());
    printer.Field("Consequent", consequent.get());
    printer.Field("Alternate", alternate.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> IfStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    auto c = condition->Execute(interpreter);
    if (c->GetType() != RuntimeValueType::Bool)
        std::exit(EXIT_FAILURE);

    auto cc = dynamic_cast<BoolValue *>(c.get());
    if (cc->GetValue())
    {
        ScopedNewScope scopeGuard(interpreter);
        consequent->Execute(interpreter);
    }
    else if (alternate)
    {
        ScopedNewScope scopeGuard(interpreter);
        alternate->Execute(interpreter);
    }

    return nullptr;
}