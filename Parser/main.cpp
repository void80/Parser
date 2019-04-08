

// STD
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>
#include <functional>


int main(int, char**)
{
    /* 
    using Parser::Symbol::Terminal;
    using Parser::Symbol::NonTerminal;
    using Parser::Rule;

    auto &eof = Parser::Symbol::s_eof;

    using namespace Parser::Action;

    Terminal id("id");
    Terminal number("number");
    Terminal plus("+");
    Terminal times("*");
    Terminal equals("=");

    NonTerminal Assign("Assign");
    NonTerminal Sums("Sums");
    NonTerminal Products("Products");
    NonTerminal Value("Value");
    
    Rule r1(&Assign,    {&id, &equals, &Sums});
    Rule r2(&Sums,      {&Sums, &plus, &Products});
    Rule r3(&Sums,      {&Products});
    Rule r4(&Products,  {&Products, &times, &Value});
    Rule r5(&Products,  {&Value});
    Rule r6(&Value,     {&number});
    Rule r7(&Value,     {&id});

    Parser::ParserTable table;
    table.setAction(&id, nullptr, std::make_unique<Shift>());
    table.setAction(&equals, &id, std::make_unique<Shift>());
    table.setAction(&id, &equals, std::make_unique<Shift>());
    
    table.setAction(&plus, &id, std::make_unique<Reduce>(r7));
    table.setAction(&plus, &Value, std::make_unique<Reduce>(r5));
    table.setAction(&plus, &Products, std::make_unique<Reduce>(r3));
    
    table.setAction(&plus, &Sums, std::make_unique<Shift>());
    table.setAction(&id, &plus, std::make_unique<Shift>());
    
    table.setAction(&times, &id, std::make_unique<Reduce>(r7));
    table.setAction(&times, &Value, std::make_unique<Reduce>(r5));
    table.setAction(&times, &Products, std::make_unique<Shift>());
    
    table.setAction(&number, &times, std::make_unique<Shift>());

    table.setAction(&eof, &number, std::make_unique<Reduce>(r6));
    table.setAction(&eof, &Value, std::make_unique<Reduce>(r4));
    table.setAction(&eof, &Products, std::make_unique<Reduce>(r2));
    table.setAction(&eof, &Sums, std::make_unique<Reduce>(r1));
    table.setAction(&eof, &Assign, std::make_unique<Accept>());



    Parser::Parser parser(std::move(table));

    std::vector<Terminal const *> input {&id, &equals, &id, &plus, &id, &times, &number};

    auto tree = parser.parse(input);
    */

    std::cout << "That's all, folks!\n";

}

