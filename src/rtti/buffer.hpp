#pragma once

#include <cstddef>
#include <cassert>

#include <core/util/res.hpp>

#include "rtti_fwd.hpp"

namespace rtti {

    using namespace core::util;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT BufferRef {
    public:
        enum class ErrData {
            INVALID_BUFFER
        };

        enum class ErrSize {
            INVALID_BUFFER
        };

        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        BufferRef() = default;
        BufferRef(const BufferRef& other) = delete;
        BufferRef(BufferRef&& other);
        BufferRef& operator=(const BufferRef& other) = delete;
        BufferRef& operator=(BufferRef&& other);

        BufferRef(void* data, size_t size);
        virtual ~BufferRef() = default;

        bool is_valid() const { return m_data && m_size; }
        Res<const void*, ErrData> data() const;
        Res<void*, ErrData> data();
        Res<void*, ErrData> steal_data() &&;
        Res<size_t, ErrSize> size() const;

        void copy(const BufferRef& src);

    protected:
        void* m_data = nullptr;
        size_t m_size = 0;

    }; // class BufferRef

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Buffer : public BufferRef {
    public:
        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        Buffer() = default;
        Buffer(const Buffer& other) = delete;
        Buffer(Buffer&& other) = default;
        Buffer& operator=(const Buffer& other) = delete;
        Buffer& operator=(Buffer&& other) = default;

        using BufferRef::BufferRef;
        Buffer(size_t size);
        ~Buffer();

        void change_buffer_size(size_t new_size);

        void release_buffer() &&;
        Object call_constructor(TypePtr type) &&;

    }; // class Buffer

} // namespace rtti