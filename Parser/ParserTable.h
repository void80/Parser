#pragma once

// PROJECT
#include "StateTable.h"
#include "State.h"


namespace Parser
{

    class ParserTable
    {
    public:
        explicit ParserTable(State startingState);
        ~ParserTable() = default;
    

        Action::Action const &action(State state, Symbol::Terminal const *input) const;
        void setAction(State state, Symbol::Terminal const *input, std::unique_ptr<Action::Action> &&action);

        State nextState(State state, Symbol::NonTerminal const *topOfStack);
        void setNextState(State state, Symbol::NonTerminal const *topOfStack, State nextState);

        State startingState() const noexcept { return m_startingState; }

    private:
        std::map<State, StateTable> m_table;
        State m_startingState;
    };

} // OF namespace
