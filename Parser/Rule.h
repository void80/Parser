#pragma once

// PROJECT
#include "symbols.h"

// STD
#include <vector>

namespace Parser
{
    class Rule final
    {
    public:
        Rule(Symbol::NonTerminal *lhs, std::vector<Symbol::Symbol *> rhs);

        std::size_t rightSideLength() const { return m_rhs.size(); }
        Symbol::NonTerminal const *lhs() const { return m_lhs; }

    private:
        Symbol::NonTerminal *m_lhs;
        std::vector<Symbol::Symbol *> m_rhs;
    };

} // OF namespaces
