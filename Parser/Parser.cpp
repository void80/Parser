
//  PROJECT
#include "Parser.h"
#include "tree.h"
#include "symbols.h"

// STD
#include <vector>
#include <memory>
#include <stack>

namespace Parser
{

    namespace 
    {
        std::vector<std::unique_ptr<Tree::Node>> extractTopOfStack(std::stack<std::unique_ptr<Tree::Node>> &stack, std::size_t const size);
        Action::Type type(Action::Action const *action);
    } // OF anonymous NS



    std::unique_ptr<Tree::Node> Parser::parse(std::vector<Symbol::Terminal const*> const &symbols)
    {
        auto state = m_table.startingState();
        std::stack<std::unique_ptr<Tree::Node>> stack;
        auto currentSymbol = symbols.begin();

        while(currentSymbol != symbols.end() || !stack.empty())
        {
            auto symbol = currentSymbol != symbols.end() ? *currentSymbol : &Symbol::eof();
            auto topOfStack = !stack.empty() ? stack.top().get() : nullptr;

            auto &action = m_table.action(state, symbol);

            switch(type(&action))
            {
            case Action::Type::Fail:
                return nullptr;

            case Action::Type::Shift:
            {
                auto shiftAction = dynamic_cast<Action::Shift const &>(action);
                stack.push(std::make_unique<Tree::TerminalNode>(symbol));
                ++currentSymbol;
                state = shiftAction.nextState();
            }
            break;

            case Action::Type::Reduce:
            {
                auto &reduceAction = dynamic_cast<Action::Reduce const &>(action);
                auto size = reduceAction.rule().rightSideLength();

                auto topOfStack = extractTopOfStack(stack, size);
                stack.push(std::make_unique<Tree::NonTerminalNode>(reduceAction.rule().lhs(), std::move(topOfStack)));
                state = m_table.nextState(state, reduceAction.rule().lhs());
            }
            break;
            case Action::Type::Accept:
                return std::move(stack.top());
            }
        }

        throw std::logic_error("Ran out of something");
    }



    namespace 
    {
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

        Action::Type type(Action::Action const *action)
        {
            using Action::Type;
            using namespace Action;
            
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




    } // OF anonymous NS
    
} // OF namespaces
