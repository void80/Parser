
// PROJECT
#include "symbols.h"


namespace Parser::Symbol
{
    Symbol::Symbol(std::string symbol)
        : m_symbol(std::move(symbol))
    {
    }

    Symbol::~Symbol() = default;


    NonTerminal::NonTerminal(std::string symbol)
        : Symbol(std::move(symbol))
    {
    }

    Terminal::Terminal(std::string symbol)
        : Symbol(std::move(symbol))
    {
    }


    Terminal const &epsilon()
    {
        static Terminal const result{std::string{}};
        return result;
    }
    
    Terminal const &eof()
    {
        static Terminal const result("$");
        return result;
    }




} // OF NS
