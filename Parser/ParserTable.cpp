
// PROJECT
#include "ParserTable.h"



namespace Parser
{
    ParserTable::ParserTable(State startingState)
        : m_startingState(startingState)
    {
    }

    Action::Action const &ParserTable::action(State state, Symbol::Terminal const *input) const
    {
        auto stateTable = m_table.find(state);
        if(stateTable != m_table.end())
        {
            return stateTable->second.action(input);
        }
        throw std::logic_error("Unknown state");
    }

    void ParserTable::setAction(State state, Symbol::Terminal const *input, std::unique_ptr<Action::Action>&& action)
    {
        return m_table[state].setAction(input, std::move(action));
    }

    State ParserTable::nextState(State state, Symbol::NonTerminal const *topOfStack)
    {
        auto stateTable = m_table.find(state);
        if(stateTable != m_table.end())
        {
            return stateTable->second.nextState(topOfStack);
        }
        throw std::logic_error("Unknown state");
    }

    void ParserTable::setNextState(State state, Symbol::NonTerminal const *topOfStack, State nextState)
    {
        return m_table[state].setNextState(topOfStack, nextState);
    }

} // OF namespace
