#pragma once

// PROJECT
#include "ParserTable.h"
#include "tree.h"

// STD
#include <memory>

namespace Parser
{

    // readily created parser to parse a string
    class Parser
    {
    public:
        explicit Parser(ParserTable table);

        std::unique_ptr<Tree::Node> parse(std::vector<Symbol::Terminal const *> const &symbols);

    private:
        ParserTable m_table;
    };

} // OF namespace

