#pragma once

// PROJECT
#include "State.h"
#include "Rule.h"

namespace Parser::Action
{
    class Action
    {
    public:
        virtual ~Action() = 0;
    };


    class Fail final : public Action
    {
    public:
        Fail();
        ~Fail() = default;
    };

    class Accept final : public Action
    {
    public:
        Accept();
        ~Accept() = default;
    };

    class Shift final : public Action
    {
    public:
        Shift(State nextState);
        ~Shift() = default;

        State nextState() const noexcept { return m_nextState; }
    private:
        State m_nextState;
    };

    class Reduce final : public Action
    {
    public:
        Reduce(Rule rule) 
            : m_rule(std::move(rule))
        {}

        ~Reduce() = default;

        Rule const &rule() const {return m_rule;}

    private:
        Rule m_rule;
    };

    enum class Type
    {
        Fail, Shift, Reduce, Accept
    };

}
