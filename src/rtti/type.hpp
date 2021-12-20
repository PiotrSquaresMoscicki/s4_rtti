#pragma once

#include <string>
#include <memory>

#include <core/util/res.hpp>

#include "rtti_fwd.hpp"
#include "attributes.hpp"

namespace rtti {

    using namespace core::util;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Type {
    public:
        enum class ErrNewObject {
            NOT_DEFAULT_CONSTRUCTIBLE
        };

        enum class ErrNewCopy {
            NOT_COPY_CONSTRUCTIBLE,
            NOT_VALID_SOURCE,
            INCORRECT_SOURCE_TYPE
        };

        enum class ErrNewMove {
            NOT_MOVE_CONSTRUCTIBLE,
            NOT_VALID_SOURCE,
            INCORRECT_SOURCE_TYPE
        };

        enum class ErrDeleteObject {
            NOT_VALID_SOURCE,
            INCORRECT_SOURCE_TYPE
        };

        enum class ErrConstruct {
            NOT_DEFAULT_CONSTRUCTIBLE,
            INVALID_BUFFER,
            BUFFER_TOO_SMALL
        };

        enum class ErrCopyConstruct {
            NOT_COPY_CONSTRUCTIBLE,
            NOT_VALID_SOURCE,
            INCORRECT_SOURCE_TYPE,
            INVALID_BUFFER,
            BUFFER_TOO_SMALL
        };

        enum class ErrMoveConstruct {
            NOT_MOVE_CONSTRUCTIBLE,
            NOT_VALID_SOURCE,
            INCORRECT_SOURCE_TYPE,
            INVALID_BUFFER,
            BUFFER_TOO_SMALL
        };

        enum class ErrDestruct {
            NOT_VALID_OBJECT,
            INCORRECT_OBJECT_TYPE
        };

        enum class ErrCopy {
            NOT_COPY_ASSIGNABLE,
            INVALID_DESTINATION_OBJECT,
            INCORRECT_DESTINATION_OBJECT_TYPE,
            INVALID_SOURCE_OBJECT,
            INCORRECT_SOURCE_OBJECT_TYPE
        };

        enum class ErrMove {
            NOT_MOVE_ASSIGNABLE,
            INVALID_DESTINATION_OBJECT,
            INCORRECT_DESTINATION_OBJECT_TYPE,
            INVALID_SOURCE_OBJECT,
            INCORRECT_SOURCE_OBJECT_TYPE
        };

        Type(std::string name, size_t size, Attributes attributes);

        const std::string& name() const { return m_name; }
        size_t size() const { return m_size; }
        const Attributes& attributes() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        virtual bool is_fundamental() const = 0;
        virtual bool is_enum() const = 0;
        virtual bool is_class() const = 0;
        virtual bool is_template_instance() const = 0;

        virtual FundamentalPtr as_fundamental() const = 0;
        virtual EnumPtr as_enum() const = 0;
        virtual ClassPtr as_class() const = 0;
        virtual TemplateInstancePtr as_template_instance() const = 0;

        virtual bool is_default_constructible() const = 0;
        virtual bool is_copy_constructible() const = 0;
        virtual bool is_move_constructible() const = 0;
        virtual bool is_copy_assignable() const = 0;
        virtual bool is_move_assignable() const = 0;

        virtual Res<Object, ErrNewObject> new_object() const = 0;
        virtual Res<Object, ErrNewCopy> new_copy(const ObjectRef& src) const = 0;
        virtual Res<Object, ErrNewMove> new_move(ObjectRef& src) const = 0;
        virtual Res<void, ErrDeleteObject> can_delete_object(const ObjectRef& obj) const = 0;
        virtual void delete_object(Object&& obj) const = 0;

        virtual Res<void, ErrConstruct> can_construct(const BufferRef& buff) const = 0;
        virtual ObjectRef construct(BufferRef&& buff) const = 0;
        virtual Object construct(Buffer&& buff) const = 0;

        virtual Res<void, ErrCopyConstruct> can_copy_construct(const BufferRef& buff
            , const ObjectRef& src) const = 0;
        virtual ObjectRef copy_construct(BufferRef&& buff, const ObjectRef& src) const = 0;
        virtual Object copy_construct(Buffer&& buff, const ObjectRef& src) const = 0;
        
        virtual Res<void, ErrMoveConstruct> can_move_construct(const BufferRef& buff
            , const ObjectRef& src) const = 0;
        virtual ObjectRef move_construct(BufferRef&& buff, ObjectRef& src) const = 0;
        virtual Object move_construct(Buffer&& buff, ObjectRef& src) const = 0;
        
        virtual Res<void, ErrDestruct> can_destruct(const ObjectRef& obj) const = 0;
        virtual BufferRef destruct(ObjectRef&& obj) const = 0;
        virtual Buffer destruct(Object&& obj) const = 0;
        
        virtual Res<void, ErrCopy> copy_assign(ObjectRef& dst, const ObjectRef& src) const = 0;
        virtual Res<void, ErrMove> move_assign(ObjectRef& dst, ObjectRef& src) const = 0;

    private:
        std::string m_name;
        size_t m_size = 0;
        Attributes m_attributes;

    }; // class Type

} // namespace rtti