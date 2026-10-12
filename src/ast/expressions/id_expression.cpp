#include "ast/expressions/id_expression.hpp"
#include "semantic_analysis/symbols/variable_symbol.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

IdExpression::IdExpression() : Expression(AstNodeType::IdExpression) {}

void IdExpression::SetValue(std::string value)
{
    this->value = value;
}

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
    auto scope = interpreter.GetCurrentScope();
    if (!scope->SymbolExists(value, SymbolKind::Variable))
    {
        std::cerr << "[IdExpression] ERROR: Lookup failed for variable " << value << std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto &symbol = scope->GetSymbol(value, SymbolKind::Variable);
    auto variableSymbol = dynamic_cast<VariableSymbol *>(symbol.get());
    return variableSymbol->GetValue()->Clone();
}