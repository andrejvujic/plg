#include "ast/statements/for_statement.hpp"

ForStatement::ForStatement() : Statement(AstNodeType::ForStatement)
{
}

void ForStatement::SetBody(std::unique_ptr<const Statement> &body)
{
    this->body = std::move(body);
}

const std::unique_ptr<const Statement> &ForStatement::GetBody() const
{
    return body;
}

void ForStatement::SetInitExpression(std::unique_ptr<const Expression> &init)
{
    this->init = std::move(init);
}

const std::unique_ptr<const Expression> &ForStatement::GetInitExpression() const
{
    return init;
}

void ForStatement::SetUpdateExpression(std::unique_ptr<const Expression> &update)
{
    this->update = std::move(update);
}

const std::unique_ptr<const Expression> &ForStatement::GetUpdateExpression() const
{
    return update;
}

void ForStatement::SetCondition(std::unique_ptr<const Expression> &condition)
{
    this->condition = std::move(condition);
}

const std::unique_ptr<const Expression> &ForStatement::GetCondition() const
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