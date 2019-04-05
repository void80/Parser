#pragma once


namespace Util
{

    template<typename T>
    class Generator
    {
    public:
        Generator(T init = T{})
            : m_next(std::move(init))
        {}

        T generate() 
        {
            auto result = m_next;
            ++m_next;
            return result;
        }

    private:
        T m_next;
    };

}

