

// STD
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>
#include <functional>

namespace Parser
{

    namespace Symbol
    {
        class Symbol
        {
        public:
            explicit Symbol(std::string symbol) 
                : m_symbol(std::move(symbol))
            {}
            virtual ~Symbol() = 0;

        private:
            std::string m_symbol;
        };

        Symbol::~Symbol() = default;

        class NonTerminal final : public Symbol
        {
        public:
            explicit NonTerminal(std::string symbol) 
                : Symbol(std::move(symbol))
            {}

            ~NonTerminal() = default;
        };

        class Terminal : public Symbol
        {
        public:
            explicit Terminal(std::string symbol) 
                : Symbol(std::move(symbol))
            {}

            ~Terminal() = default;
        };

        static Terminal const s_epsilon{std::string{}};
        static Terminal const s_eof("$");

    }

    namespace Tree
    {
        class Node
        {
        public:
            virtual ~Node() = 0;

            virtual std::unique_ptr<Node> clone() const = 0;
            virtual Symbol::Symbol const *symbol() const = 0;
        };

        Node::~Node() = default;

        class NonTerminalNode final : public Node
        {
        public:
            explicit NonTerminalNode(Symbol::NonTerminal const *symbol)
                : m_symbol(symbol)
                , m_children()
            {}
            
            NonTerminalNode(Symbol::NonTerminal const *symbol, std::vector<std::unique_ptr<Node>> &&children)
                : m_symbol(symbol)
                , m_children(std::move(children))
            {}
            
            ~NonTerminalNode() final = default;

            std::unique_ptr<Node> clone() const override
            {
                std::vector<std::unique_ptr<Node>> copy;
                std::transform(m_children.begin(), m_children.end(), std::back_inserter(copy), std::mem_fn(&Node::clone));
                return std::make_unique<NonTerminalNode>(m_symbol, std::move(copy));
            }
        
            Symbol::NonTerminal const *symbol() const override { return m_symbol; }

        private:
            Symbol::NonTerminal const *m_symbol;
            std::vector<std::unique_ptr<Node>> m_children;
        };

        class TerminalNode final : public Node
        {
        public:
            TerminalNode(Symbol::Terminal const *symbol)
                : m_symbol(symbol)
            {}
            ~TerminalNode() final = default;

            std::unique_ptr<Node> clone() const override
            {
                return std::make_unique<TerminalNode>(m_symbol);
            }

            Symbol::Terminal const *symbol() const override { return m_symbol; }
        
        private:
            Symbol::Terminal const *m_symbol;
        
        };
    }
    std::vector<std::unique_ptr<Tree::Node>> extractTopOfStack(std::stack<std::unique_ptr<Tree::Node>> &stack, size_t const size);

    class Rule final
    {
    public:
        Rule(Symbol::NonTerminal *lhs, std::vector<Symbol::Symbol *> rhs)
            : m_lhs(lhs)
            , m_rhs(std::move(rhs))
        {}

        std::size_t rightSideLength() const { return m_rhs.size(); }
        Symbol::NonTerminal const *lhs() const { return m_lhs; }

    private:
        Symbol::NonTerminal *m_lhs;
        std::vector<Symbol::Symbol *> m_rhs;
    };

    namespace Action
    {
        class Action
        {
        public:
            virtual ~Action() = 0;
        };
        
        Action::~Action() = default;

        class Fail final : public Action
        {
        public:
            Fail() {}
            ~Fail() = default;
        };
        class Accept final : public Action
        {
        public:
            Accept() {}
            ~Accept() = default;
        };

        class Shift final : public Action
        {
        public:
            Shift() {}
            ~Shift() = default;
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

        Type type(Action const *action)
        {
            if(action == nullptr)
            {
                return Type::Fail; // Fail?
            }
            else if(dynamic_cast<Shift const *>(action) != nullptr)
            {
                return Type::Shift;
            }
            else if(dynamic_cast<Reduce const *>(action) != nullptr)
            {
                return Type::Reduce;
            }
            else if(dynamic_cast<Accept const *>(action) != nullptr)
            {
                return Type::Accept;
            }
            else if(dynamic_cast<Fail const *>(action) != nullptr)
            {
                return Type::Fail;
            }
            else
            {
                throw std::logic_error("Unknown type");
            }
        }


    }



    class ParserTable
    {
    public:
        Action::Action const &action(Symbol::Terminal const *input, Symbol::Symbol const *topOfStack) const // TODO: use gsl::not_null
        {
            auto outer = m_table.find(input);
            if(outer != m_table.end())
            {
                auto inner = outer->second.find(topOfStack);
                if(inner != outer->second.end())
                {
                    return *inner->second;
                }
            }
            return m_fail;
        }

        void setAction(Symbol::Terminal const *input, Symbol::Symbol const *topOfStack, std::unique_ptr<Action::Action> &&action)
        {
            auto outer = m_table.find(input);
            if(outer == m_table.end())
            {
                outer = m_table.emplace(input, std::map<Symbol::Symbol const *, std::unique_ptr<Action::Action>>()).first;
            }

            auto inner = outer->second.find(topOfStack);
            if(inner == outer->second.end())
            {
                outer->second.emplace(topOfStack, std::move(action));
            }
            else
            {
                throw std::logic_error("Duplicate entry in parser table");
            }
        }


    private:
        std::map<Symbol::Terminal const *, std::map<Symbol::Symbol const *, std::unique_ptr<Action::Action>>> m_table;
        Action::Fail m_fail;
    };


    // readily created parser to parse a string
    class Parser
    {
    public:
        explicit Parser(ParserTable table)
            : m_table(std::move(table))
        {}

        std::unique_ptr<Tree::Node> parse(std::vector<Symbol::Terminal const *> const &symbols)
        {
            std::stack<std::unique_ptr<Tree::Node>> stack;
            auto currentSymbol = symbols.begin();

            while(currentSymbol != symbols.end() || !stack.empty())
            {
                auto symbol = currentSymbol != symbols.end() ? *currentSymbol : &Symbol::s_eof;
                auto topOfStack = !stack.empty() ? stack.top().get() : nullptr;

                auto &action = m_table.action(symbol, topOfStack != nullptr ? topOfStack->symbol() : nullptr);

                switch(type(&action))
                {
                case Action::Type::Fail:
                    return nullptr;
                
                case Action::Type::Shift:
                    stack.push(std::make_unique<Tree::TerminalNode>(symbol));
                    ++currentSymbol;
                    break;

                case Action::Type::Reduce:
                    {
                        auto &reduceAction = dynamic_cast<Action::Reduce const &>(action);
                        auto size =  reduceAction.rule().rightSideLength();
                    
                        auto topOfStack = extractTopOfStack(stack, size);
                        stack.push(std::make_unique<Tree::NonTerminalNode>(reduceAction.rule().lhs(), std::move(topOfStack)));
                    }
                    break;
                case Action::Type::Accept:
                    return std::move(stack.top());
                }
            }

            throw std::logic_error("Ran out of something");
        }

    private:
        // std::vector<
        ParserTable m_table;
    };

    std::vector<std::unique_ptr<Tree::Node>> extractTopOfStack(std::stack<std::unique_ptr<Tree::Node>> &stack, size_t const size)
    {
        std::vector<std::unique_ptr<Tree::Node>> result;
        result.reserve(size);
        for(std::size_t i = 0; i < size; ++i)
        {
            result.emplace_back(std::move(stack.top()));
            stack.pop();
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

}

int main(int, char**)
{
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

    std::cout << "That's all, folks!\n";

}

