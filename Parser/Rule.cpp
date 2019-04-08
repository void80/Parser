

// PROJECT
#include "Rule.h"


namespace Parser
{

    Rule::Rule(Symbol::NonTerminal *lhs, std::vector<Symbol::Symbol*> rhs)
        : m_lhs(lhs)
        , m_rhs(std::move(rhs))
    {
    }

} // OF namespaces
