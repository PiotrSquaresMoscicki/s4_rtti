#pragma once

#include <cstddef>
#include <cassert>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT BufferRef {
    public:
        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        BufferRef() = default;
        BufferRef(size_t size);

        bool is_valid() const { return m_data && m_size; }
        const void* data() const { assert(is_valid()); return m_data; }
        void* data() { assert(is_valid()); return m_data; }
        size_t size() const { assert(is_valid()); return m_size; }

        virtual Object call_constructor(const Type* type) &&;
        virtual void copy(const BufferRef& src);
        virtual void move(BufferRef&& src);

    private:
        void* m_data = nullptr;
        size_t m_size = 0;

    }; // class BufferRef

} // namespace rtti