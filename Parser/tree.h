#pragma once

// PROJECT
#include "symbols.h"

// STD
#include <memory>
#include <vector>

namespace Parser::Tree
{
    class Node
    {
    public:
        virtual ~Node() = 0;

        virtual std::unique_ptr<Node> clone() const = 0;
        virtual Symbol::Symbol const *symbol() const = 0; // TODO: still needed?
    };


    class NonTerminalNode final : public Node
    {
    public:
        explicit NonTerminalNode(Symbol::NonTerminal const *symbol);
        NonTerminalNode(Symbol::NonTerminal const *symbol, std::vector<std::unique_ptr<Node>> &&children);
        ~NonTerminalNode() final = default;

    public:
        std::unique_ptr<Node> clone() const final;
        Symbol::NonTerminal const *symbol() const final { return m_symbol; }

    private:
        Symbol::NonTerminal const *m_symbol;
        std::vector<std::unique_ptr<Node>> m_children;
    };

    class TerminalNode final : public Node
    {
    public:
        explicit TerminalNode(Symbol::Terminal const *symbol);
        ~TerminalNode() final = default;

    public:
        std::unique_ptr<Node> clone() const final;
        Symbol::Terminal const *symbol() const final { return m_symbol; }

    private:
        Symbol::Terminal const *m_symbol;
    };

}
