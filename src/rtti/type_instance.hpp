#pragma once

#include <cassert>
#include <vector>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    using namespace core::util;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename TYPE>
    class TypeInstance : public virtual Type {
    public:
        bool is_default_constructible() const override;
        bool is_destructible() const override;
        bool is_copy_constructible() const override;
        bool is_move_constructible() const override;
        bool is_copy_assignable() const override;
        bool is_move_assignable() const override;

        Res<void, ErrConstruct> can_construct() const override;
        Res<void, ErrConstruct> can_construct(const BufferRef& buff) const override;
        Res<Object, ErrConstruct> alloc_construct() const override;
        Res<ObjectRef, ErrConstruct> construct(BufferRef&& buff) const override;
        Res<Object, ErrConstruct> construct(Buffer&& buff) const override;

        Res<void, ErrCopyConstruct> can_copy_construct(const ObjectRef& src) const override;
        Res<void, ErrCopyConstruct> can_copy_construct(const BufferRef& buff
            , const ObjectRef& src) const override;
        Res<Object, ErrCopyConstruct> alloc_copy_construct(const ObjectRef& src) const override;
        Res<ObjectRef, ErrCopyConstruct> copy_construct(BufferRef&& buff
            , const ObjectRef& src) const override;
        Res<Object, ErrCopyConstruct> copy_construct(Buffer&& buff
            , const ObjectRef& src) const override;
        
        Res<void, ErrMoveConstruct> can_move_construct(const ObjectRef& src) const override;
        Res<void, ErrMoveConstruct> can_move_construct(const BufferRef& buff
            , const ObjectRef& src) const override;
        Res<Object, ErrMoveConstruct> alloc_move_construct(ObjectRef& src) const override;
        Res<ObjectRef, ErrMoveConstruct> move_construct(BufferRef&& buff
            , ObjectRef& src) const override;
        Res<Object, ErrMoveConstruct> move_construct(Buffer&& buff
            , ObjectRef& src) const override;
        
        Res<void, ErrDestruct> can_destruct(const ObjectRef& obj) const override;
        Res<void, ErrDestruct> dealloc_destruct(Object&& obj) const override;
        Res<BufferRef, ErrDestruct> destruct(ObjectRef&& obj) const override;
        Res<Buffer, ErrDestruct> destruct(Object&& obj) const override;

        Res<void, ErrCopy> can_copy_assign(const ObjectRef& dst
            , const ObjectRef& src) const override;
        Res<void, ErrCopy> copy_assign(ObjectRef& dst, const ObjectRef& src) const override;

        Res<void, ErrMove> can_move_assign(const ObjectRef& dst
            , const ObjectRef& src) const override;
        Res<void, ErrMove> move_assign(ObjectRef& dst, ObjectRef& src) const override;

    }; // class TypeInstance

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_default_constructible() const {
        return std::is_default_constructible_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_destructible() const {
        return std::is_destructible_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_copy_constructible() const {
        return std::is_copy_constructible_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_move_constructible() const {
        return std::is_move_constructible_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_copy_assignable() const {
        return std::is_copy_assignable_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    bool TypeInstance<TYPE>::is_move_assignable() const {
        return std::is_move_assignable_v<TYPE>;
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrConstruct> TypeInstance<TYPE>::can_construct() const 
    {
        if constexpr (!std::is_default_constructible_v<TYPE>)
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrConstruct> TypeInstance<TYPE>::can_construct(
        const BufferRef& buff) const 
    {
        if constexpr (!std::is_default_constructible_v<TYPE>)
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrConstruct::BUFFER_TOO_SMALL);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrConstruct> TypeInstance<TYPE>::alloc_construct() const {
        if constexpr (!std::is_default_constructible_v<TYPE>)
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else
            return Ok(Object(new TYPE()));
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<ObjectRef, Type::ErrConstruct> TypeInstance<TYPE>::construct(BufferRef&& buff) const {
        if constexpr (!std::is_default_constructible_v<TYPE>)
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrConstruct::BUFFER_TOO_SMALL);
        else
            return Ok(ObjectRef(new(buff.data().ok()) TYPE, buff.size().ok()));
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrConstruct> TypeInstance<TYPE>::construct(Buffer&& buff) const {
        if constexpr (!std::is_default_constructible_v<TYPE>)
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrConstruct::BUFFER_TOO_SMALL);
        else {
            Object res(new(buff.data().ok()) TYPE, buff.size().ok());
            std::move(buff).steal_data();
            return Ok(std::move(res));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrCopyConstruct> TypeInstance<TYPE>::can_copy_construct(
        const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_constructible_v<TYPE>)
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrCopyConstruct> TypeInstance<TYPE>::can_copy_construct(
        const BufferRef& buff, const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_constructible_v<TYPE>)
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrCopyConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrCopyConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrCopyConstruct> TypeInstance<TYPE>::alloc_copy_construct(
        const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_constructible_v<TYPE>)
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new TYPE(*src.value_as<TYPE>().ok())));
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<ObjectRef, Type::ErrCopyConstruct> TypeInstance<TYPE>::copy_construct(BufferRef&& buff
        , const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_constructible_v<TYPE>)
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrCopyConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrCopyConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else {
            return Ok(ObjectRef(
                new(buff.data().ok()) TYPE(*src.value_as<TYPE>().ok()), buff.size().ok()));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrCopyConstruct> TypeInstance<TYPE>::copy_construct(Buffer&& buff
        , const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_constructible_v<TYPE>)
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrCopyConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrCopyConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else {
            Object res(
                new(buff.data().ok()) TYPE(*src.value_as<TYPE>().ok()), buff.size().ok());
            std::move(buff).steal_data();
            return Ok(std::move(res));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrMoveConstruct> TypeInstance<TYPE>::can_move_construct(
        const ObjectRef& src) const 
    {
        if constexpr (!std::is_move_constructible_v<TYPE>)
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrMoveConstruct> TypeInstance<TYPE>::can_move_construct(
        const BufferRef& buff, const ObjectRef& src) const 
    {
        if constexpr (!std::is_move_constructible_v<TYPE>)
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrMoveConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrMoveConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrMoveConstruct> TypeInstance<TYPE>::alloc_move_construct(
        ObjectRef& src) const 
    {
        if constexpr (!std::is_move_constructible_v<TYPE>)
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new TYPE(std::move(*src.value_as<TYPE>().ok()))));
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<ObjectRef, Type::ErrMoveConstruct> TypeInstance<TYPE>::move_construct(BufferRef&& buff
        , ObjectRef& src) const 
    {
        if constexpr (!std::is_move_constructible_v<TYPE>)
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrMoveConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrMoveConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else {
            return Ok(ObjectRef(
                new(buff.data().ok()) 
                TYPE(std::move(*src.value_as<TYPE>().ok())), buff.size().ok()));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Object, Type::ErrMoveConstruct> TypeInstance<TYPE>::move_construct(Buffer&& buff
        , ObjectRef& src) const 
    {
        if constexpr (!std::is_move_constructible_v<TYPE>)
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrMoveConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrMoveConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else {
            Object res(
                new(buff.data().ok()) 
                TYPE(std::move(*src.value_as<TYPE>().ok())), buff.size().ok());
            std::move(buff).steal_data();
            return Ok(std::move(res));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrDestruct> TypeInstance<TYPE>::can_destruct(
        const ObjectRef& obj) const 
    {
        if constexpr (!std::is_destructible_v<TYPE>)
            return Err(ErrDestruct::NOT_DESTRUCTIBLE);
        else if (!obj.is_valid())
            return Err(ErrDestruct::NOT_VALID_OBJECT);
        else if (obj.type().ok() != TypePtr(this))
            return Err(ErrDestruct::INCORRECT_OBJECT_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrDestruct> TypeInstance<TYPE>::dealloc_destruct(Object&& obj) const {
        if constexpr (!std::is_destructible_v<TYPE>)
            return Err(ErrDestruct::NOT_DESTRUCTIBLE);
        else if (!obj.is_valid())
            return Err(ErrDestruct::NOT_VALID_OBJECT);
        else if (obj.type().ok() != TypePtr(this))
            return Err(ErrDestruct::INCORRECT_OBJECT_TYPE);
        else {

// classes registered in the rtti system have vtables but destructor is not created implicitly
// this is not a problem since we don't add any data that would need to be cleaned up in the 
// destructor of the base class
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
            delete reinterpret_cast<TYPE*>(obj.m_value);
#pragma GCC diagnostic pop

            obj.m_value = nullptr;
            obj.m_type = nullptr;

            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<BufferRef, Type::ErrDestruct> TypeInstance<TYPE>::destruct(ObjectRef&& obj) const {
        if constexpr (!std::is_destructible_v<TYPE>)
            return Err(ErrDestruct::NOT_DESTRUCTIBLE);
        else if (!obj.is_valid())
            return Err(ErrDestruct::NOT_VALID_OBJECT);
        else if (obj.type().ok() != TypePtr(this))
            return Err(ErrDestruct::INCORRECT_OBJECT_TYPE);
        else
        {
            reinterpret_cast<TYPE*>(obj.value().ok())->~TYPE();
            BufferRef res(obj.value().ok(), obj.size().ok());
            std::move(obj).steal_value();
            return Ok(std::move(res));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<Buffer, Type::ErrDestruct> TypeInstance<TYPE>::destruct(Object&& obj) const {
        if constexpr (!std::is_destructible_v<TYPE>)
            return Err(ErrDestruct::NOT_DESTRUCTIBLE);
        else if (!obj.is_valid())
            return Err(ErrDestruct::NOT_VALID_OBJECT);
        else if (obj.type().ok() != TypePtr(this))
            return Err(ErrDestruct::INCORRECT_OBJECT_TYPE);
        else {
            reinterpret_cast<TYPE*>(obj.value().ok())->~TYPE();
            Buffer res(obj.value().ok(), obj.size().ok());
            std::move(obj).steal_value();
            return Ok(std::move(res));
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrCopy> TypeInstance<TYPE>::can_copy_assign(const ObjectRef& dst
        , const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_assignable_v<TYPE>)
            return Err(Type::ErrCopy::NOT_COPY_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrCopy::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != TypePtr(this))
            return Err(Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrCopy::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrCopy> TypeInstance<TYPE>::copy_assign(ObjectRef& dst
        , const ObjectRef& src) const 
    {
        if constexpr (!std::is_copy_assignable_v<TYPE>)
            return Err(Type::ErrCopy::NOT_COPY_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrCopy::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != TypePtr(this))
            return Err(Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrCopy::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            *reinterpret_cast<TYPE*>(dst.value().ok()) 
                = *reinterpret_cast<const TYPE*>(src.value().ok());
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrMove> TypeInstance<TYPE>::can_move_assign(const ObjectRef& dst
        , const ObjectRef& src) const 
    {
        if constexpr (!std::is_move_assignable_v<TYPE>)
            return Err(Type::ErrMove::NOT_MOVE_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrMove::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != TypePtr(this))
            return Err(Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrMove::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename TYPE>
    Res<void, Type::ErrMove> TypeInstance<TYPE>::move_assign(ObjectRef& dst
        , ObjectRef& src) const 
    {
        if constexpr (!std::is_move_assignable_v<TYPE>)
            return Err(Type::ErrMove::NOT_MOVE_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrMove::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != TypePtr(this))
            return Err(Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrMove::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            TYPE& dst_ref = *reinterpret_cast<TYPE*>(dst.value().ok());
            TYPE& src_ref = *reinterpret_cast<TYPE*>(src.value().ok());
            dst_ref = std::move(src_ref);
            return Ok();
        }
    }

} // namespace rtti