#pragma once

#include <string>
#include <memory>

#include "rtti_fwd.hpp"
#include "attributes.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Type {
    public:
        enum class ErrConstruct {
            INVALID_BUFFER
        };

        enum class ErrCopyConstruct {
            INVALID_BUFFER,

        }

        Type(std::string name, size_t size, Attributes attributes);

        const std::string& name() const { return m_name; }
        size_t size() const { return m_size; }
        const Attributes& attributes() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        virtual Object new_object() const = 0;
        virtual Object new_copy(const ObjectRef& src) const = 0;
        virtual Object new_move(ObjectRef& src) const = 0;
        virtual void delete_object(Object&& obj) const = 0;

        bool is_copyable() const;
        bool is_movable() const;

        virtual Res<void, ErrConstruct> can_construct(const BufferRef& buff) const = 0;
        virtual ObjectRef construct(BufferRef&& buff) const = 0;
        virtual Object construct(Buffer&& buff) const = 0;
        virtual bool can_copy_construct(const BufferRef& buff, const ObjectRef& src) const = 0;
        virtual ObjectRef copy_construct(BufferRef&& buff, const ObjectRef& src) const = 0;
        virtual Object copy_construct(Buffer&& buff, const ObjectRef& src) const = 0;
        virtual ObjectRef move_construct(BufferRef&& buff, ObjectRef& src) const = 0;
        virtual Object move_construct(Buffer&& buff, ObjectRef& src) const = 0;
        virtual BufferRef destruct(ObjectRef&& obj) const = 0;
        virtual Buffer destruct(Object&& obj) const = 0;
        
        virtual void copy(ObjectRef& dst, const ObjectRef& src) const = 0;
        virtual void move(ObjectRef& dst, ObjectRef& src) const = 0;

        virtual const Fundamental* as_fundamental() const = 0;
        virtual const Enum* as_enum() const = 0;
        virtual const Class* as_class() const = 0;

    private:
        std::string m_name;
        size_t m_size = 0;
        Attributes m_attributes;

    }; // class Type

} // namespace rtti