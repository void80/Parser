#pragma once


namespace Parser
{
    class State
    {
    public:
        explicit State(int index = 0) 
            : m_index(index)
        {}

        friend bool operator==(State lhs, State rhs)
        {
            return lhs.m_index == rhs.m_index;
        }
        friend bool operator!=(State lhs, State rhs)
        {
            return !(lhs == rhs);
        }

        State &operator++()
        {
            ++m_index;
            return *this;
        }

    private:
        int m_index;
    };
}
