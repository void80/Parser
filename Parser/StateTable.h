#pragma once


// PROJECT
#include "actions.h"

// STD
#include <map>

namespace Parser
{
    // Table of actions and gotos in one state
    class StateTable
    {
    public:
        Action::Action const &action(Symbol::Terminal const *input) const; // TODO: use gsl::not_null
        void setAction(Symbol::Terminal const *input, std::unique_ptr<Action::Action> &&action);

        State nextState(Symbol::NonTerminal const *topOfStack);
        void setNextState(Symbol::NonTerminal const *topOfStack, State nextState);


    private:
        std::map<Symbol::Terminal const *, std::unique_ptr<Action::Action>> m_actions;
        std::map<Symbol::NonTerminal const *, State> m_gotos;
        Action::Fail m_fail;
    };

} // OF namespaces
