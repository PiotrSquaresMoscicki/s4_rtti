#pragma once

#include <vector>

#include "rtti.hpp"

namespace script {

    using namespace rtti;
    using namespace core::str;

    class Program;
    class ProgramRef;

    class StackRef {
    public:
        void push(TypePtr type);
        void pop();
        ObjectRef get(size_t idx);

    private:
        std::vector<ObjectRef> m_variables;

    }; // class StackRef

    class Stack : public StackRef {

    };

    class ScriptStep {
    public:
        enum class Operation {
            INVALID,
            PUSH,
            POP,
            CALL,
            COPY_ASSIGN
        };

        static ScriptStep push(StringId type);
        static ScriptStep pop();
        static ScriptStep call(StringId function);
        static ScriptStep copy_assign(size_t dst, size_t src);
        static ScriptStep copy_assign(size_t dst, const ObjectRef& src);

        void step(StackRef& stack) const {}

    private:
        const Operation m_operation = Operation::INVALID;
        const TypePtr m_type;
        const Function* m_function = nullptr;
        const size_t m_stack_dst = 0;
        const size_t m_stack_src = 0;
        const ObjectRef m_heap_dst;
        const ObjectRef m_heap_src;

    };

    class Script {
    public:
        Program run() const;
        ProgramRef run(BufferRef&& buff) const;

        bool step(StackRef& stack, size_t step) const {
            if (step >= m_steps.size())
                return false;

            m_steps[step].step(stack);
            return true;
        }

    private:
        std::vector<ScriptStep> m_steps;

    };

    class ProgramRef {
    public:
        ProgramRef(const Script* script, StackRef&& stack) 
            : m_script(script), m_stack(stack) {}

        void execute() {
            while(step());
        }

        bool step() {
            return m_script->step(m_stack, m_last_script_step++);
        }

    private:
        const Script* m_script = nullptr;
        StackRef m_stack;
        size_t m_last_script_step = 0;

    };

/*

int integer;                                            
    # push_variable
int another_integer = 8;                                
    # push_variable
    # call copy ctor
int result = sum(integer, 4);                           
    # push_variable(result)
    # push_variable(integer)
    # push 
result = sum(result, sum(integer, another_integer));

*/

/*

std::vector<ScriptStep> g_steps = {
    // int integer;
    ScriptStep::push("int"),

    // int another_integer = 8;
    ScriptStep::push("int"),
    ScriptStep::copy_assign(1, Object(8)),

    // int result = sum(integer, 4);
    ScriptStep::push("int"), // result
    ScriptStep::push("int"), // zero param - space for return value
    ScriptStep::push("int"), // first param
    ScriptStep::copy_assign(4, 0), // first param = integer
    ScriptStep::push("int"), // second param
    ScriptStep::copy_assign(5, Object(4)), // second param = 4
    ScriptStep::call("sum"),
        // sum takes 3 params: 
        //  1. zero param - result variable value or address, 
        //  2. first param value or address, 
        //  3. second param value or address
        // so there is no need to copy the return value to the result
        // but that's an optimization so i'll copy it anyway for now
    ScriptStep::copy_assign(2, 3), // result = return value
    ScriptStep::pop(), // pop second param
    ScriptStep::pop(), // pop first param
    ScriptStep::pop(), // pop zero param

    // result = sum(result, sum(integer, another_integer));
    ScriptStep::push("int"), // zero param
    ScriptStep::push("int"), // first param
    ScriptStep::copy_assign(4, 2), // first param = result
    ScriptStep::push("int"), // second param
    // sum(integer, another_integer)
    ScriptStep::push("int"), // zero param for nested sum
    ScriptStep::push("int"), // first param for nested sum
    ScriptStep::copy_assign(7, 0), // first param for nested sum = integer
    ScriptStep::push("int"), // second param for nested sum
    ScriptStep::copy_assign(8, 1), // second param for nested sum = another_integer
    ScriptStep::call("sum"), // call nested sum
    ScriptStep::copy_assign(5, 6), // second param = zero param for nested sum
    ScriptStep::pop(), // pop second param for nested sum
    ScriptStep::pop(), // pop first param for nested sum
    ScriptStep::pop(), // pop zero param for nested sum
    ScriptStep::call("sum"),
    ScriptStep::copy_assign(2, 3)

};
*/
} // namespace script