#include "ast/statements/for_statement.hpp"

#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/bool_value.hpp"
#include "semantic_analysis/scoped_new_scope.hpp"

ForStatement::ForStatement() : Statement(AstNodeType::ForStatement)
{
}

void ForStatement::SetBody(std::unique_ptr<Statement> &body)
{
    this->body = std::move(body);
}

const std::unique_ptr<Statement> &ForStatement::GetBody() const
{
    return body;
}

void ForStatement::SetInitExpression(std::unique_ptr<Expression> &init)
{
    this->init = std::move(init);
}

const std::unique_ptr<Expression> &ForStatement::GetInitExpression() const
{
    return init;
}

void ForStatement::SetUpdateExpression(std::unique_ptr<Expression> &update)
{
    this->update = std::move(update);
}

const std::unique_ptr<Expression> &ForStatement::GetUpdateExpression() const
{
    return update;
}

void ForStatement::SetCondition(std::unique_ptr<Expression> &condition)
{
    this->condition = std::move(condition);
}

const std::unique_ptr<Expression> &ForStatement::GetCondition() const
{
    return condition;
}

void ForStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ForStatement");
    printer.Field("Init", init.get());
    printer.Field("Condition", condition.get());
    printer.Field("Update", update.get());
    printer.Field("Body", body.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> ForStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    init->Execute(interpreter);
    for (;;)
    {
        auto c = condition->Execute(interpreter);
        auto cc = dynamic_cast<BoolValue *>(c.get());

        if (!cc->GetValue())
            break;

        ScopedNewScope scopeGuard(interpreter);
        body->Execute(interpreter);
        update->Execute(interpreter);
    }
    return nullptr;
}
