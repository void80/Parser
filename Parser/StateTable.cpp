// PROJECT
#include "StateTable.h"

namespace Parser
{

    Action::Action const & Parser::StateTable::action(Symbol::Terminal const *input) const 
    {
        auto iter = m_actions.find(input);
        if(iter != m_actions.end())
        {
            return *iter->second;
        }
        return m_fail;
    }

    void Parser::StateTable::setAction(Symbol::Terminal const * input, std::unique_ptr<Action::Action>&& action)
    {
        if(m_actions.find(input) == m_actions.end())
        {
            m_actions.emplace(input, std::move(action));
        }
        else
        {
            throw std::logic_error("Duplicate entry in state table");
        }
    }

    State StateTable::nextState(Symbol::NonTerminal const *topOfStack)
    {
        auto iter = m_gotos.find(topOfStack);
        if(iter != m_gotos.end())
        {
            return iter->second;
        }
        throw std::logic_error("What is this?");
    }

    void StateTable::setNextState(Symbol::NonTerminal const *topOfStack, State nextState)
    {
        if(m_gotos.find(topOfStack) == m_gotos.end())
        {
            m_gotos.emplace(topOfStack, nextState);
        }
        else
        {
            throw std::logic_error("Duplicate entry in state table");
        }
    }
    
} // OF namespaces
