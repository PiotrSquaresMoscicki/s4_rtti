#pragma once

#include <cstddef>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Buffer {
    public:
        friend Type;

        size_t size() const { return m_size; }
        const void* data() const { return m_data; }

        virtual Object call_constructor(const Type* type) &&;
        virtual void copy(const Buffer& src);
        virtual void move(Buffer&& src);

    private:
        size_t m_size = 0;
        void* m_data = 0;

    }; // class Buffer

} // namespace rtti