
    class StackRef {

    };

    class Stack : public StackRef {

    };

    class ScriptVariable {

    };

    class ScriptStep {
        void call(const std::vector)
    }

    class Script {
    public:
        Stack enter() const;
        StackRef enter(BufferRef&& buff) const;
        void leave(Stack&& stack) const;
        BufferRef leave(StackRef&& stack) const;

        void step(StackRef& stack) const;

    private:
        std::vector<ScriptVariable> m_variables;
        std::vector<ScriptStep> m_steps;

    };

    class Call {
    public:

    private:
        const Method* m_method = nullptr;

    }