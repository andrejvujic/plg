#include "ast/statements/while_statement.hpp"
#include "runtime/scoped_environment.hpp"
#include "runtime/values/bool_value.hpp"

const std::unique_ptr<Expression> &WhileStatement::GetCondition() const
{
    return condition;
}

const std::unique_ptr<Statement> &WhileStatement::GetBody() const
{
    return body;
}

std::unique_ptr<RuntimeValue> WhileStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    // Without this nything defined in condition will belong to parent scope.
    // ScopedEnvironment e(interpreter);
    for (;;)
    {
        auto c = condition->Execute(interpreter);
        auto cc = dynamic_cast<BoolValue *>(c.get());
        if (!cc->GetValue())
            break;

        ScopedEnvironment e(interpreter);
        body->Execute(interpreter);
    }

    return nullptr;
}

void WhileStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("WhileStatement");
    printer.Field("Condition", condition.get());
    printer.Field("Body", body.get());
    printer.EndNode();
}