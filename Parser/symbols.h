#pragma once


// STD
#include <string>

namespace Parser::Symbol
{
    class Symbol
    {
    public:
        explicit Symbol(std::string symbol);
        virtual ~Symbol() = 0;

    private:
        std::string m_symbol;
    };

    class NonTerminal final : public Symbol
    {
    public:
        explicit NonTerminal(std::string symbol);
        ~NonTerminal() = default;
    };

    class Terminal : public Symbol
    {
    public:
        explicit Terminal(std::string symbol);
        ~Terminal() = default;
    };
    
    Terminal const &epsilon();
    Terminal const &eof();

}

