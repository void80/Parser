
// PROJECT
#include "actions.h"

namespace Parser::Action
{

    Action::~Action() = default;


    Fail::Fail()
    {
    }

    Accept::Accept()
    {
    }

    Shift::Shift(State nextState) 
        : m_nextState(nextState)
    {
    }

} // OF Namespaces
