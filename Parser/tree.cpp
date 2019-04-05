
// PROJECT
#include "tree.h"

// STD
#include <algorithm>
#include <functional>

namespace Parser::Tree
{
    Node::~Node() = default;

    NonTerminalNode::NonTerminalNode(Symbol::NonTerminal const * symbol)
        : m_symbol(symbol)
        , m_children()
    {
    }

    NonTerminalNode::NonTerminalNode(Symbol::NonTerminal const * symbol, std::vector<std::unique_ptr<Node>>&& children)
        : m_symbol(symbol)
        , m_children(std::move(children))
    {
    }

    std::unique_ptr<Node> NonTerminalNode::clone() const
    {
        std::vector<std::unique_ptr<Node>> copy;
        std::transform(m_children.begin(), m_children.end(), std::back_inserter(copy), std::mem_fn(&Node::clone));
        return std::make_unique<NonTerminalNode>(m_symbol, std::move(copy));
    }

    TerminalNode::TerminalNode(Symbol::Terminal const * symbol)
        : m_symbol(symbol)
    {
    }

    std::unique_ptr<Node> TerminalNode::clone() const
    {
        return std::make_unique<TerminalNode>(m_symbol);
    }

} // OF Namespaces
