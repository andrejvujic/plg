#include "parser/ast/ast_printer.hpp"
#include "parser/ast/ast.hpp"

AstPrinter::AstPrinter(std::ostream &o) : out(o) {}

void AstPrinter::BeginNode(std::string_view name)
{
    if (indentBeginNode)
        out << std::string(depth * 2, ' ');
    out << "[" << name << "] {\n";
    indentBeginNode = true;
    ++depth;
}

void AstPrinter::Field(std::string_view name, std::string_view value)
{
    out << std::string(depth * 2, ' ') << name << " -> " << value << "\n";
}

void AstPrinter::Field(std::string_view name, const AstNode *node)
{
    out << std::string(depth * 2, ' ') << name << " -> ";
    if (node)
    {
        indentBeginNode = false;
        node->Print(*this);
    }
    else
    {
        out << "null\n";
    }
}

void AstPrinter::EndNode()
{
    --depth;
    out << std::string(depth * 2, ' ') << "}\n";
}
