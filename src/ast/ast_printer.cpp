#include "ast/ast_printer.hpp"
#include "ast/ast.hpp"

void AstPrinter::BeginNode(std::string_view name)
{
    if (isAtLineStart)
        out << std::string(depth * 2, ' ');
    out << "[" << name << "] {\n";
    isAtLineStart = true;
    ++depth;
}

void AstPrinter::Field(std::string_view name, std::string_view value)
{
    out << std::string(depth * 2, ' ') << name << " -> " << value << "\n";
}

void AstPrinter::Field(std::string_view name, int value)
{
    out << std::string(depth * 2, ' ') << name << " -> " << value << "\n";
}

void AstPrinter::Field(std::string_view name, const AstNode *node)
{

    if (node)
    {
        out << std::string(depth * 2, ' ') << name << " -> ";
        isAtLineStart = false;
        node->Print(*this);
    }
    else if (!skipNull)
    {
        out << std::string(depth * 2, ' ') << name << " -> ";
        out << "null\n";
    }
}

void AstPrinter::EndNode()
{
    --depth;
    out << std::string(depth * 2, ' ') << "}\n";
}
