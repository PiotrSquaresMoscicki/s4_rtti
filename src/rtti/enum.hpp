#pragma once

#include <vector>
#include <cassert>

#include "type.hpp"
#include "object.hpp"
#include "buffer.hpp"

namespace rtti {

    class EnumValue;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Enum : public Type {
    public:
        Enum(std::string name, size_t size, std::vector<EnumValue> values);

        const std::vector<const EnumValue*>& values() const { return m_values_ptrs; }

        const Fundamental* as_fundamental() const override { return nullptr; }
        const Enum* as_enum() const override { return this; }
        const Class* as_class() const override { return nullptr; }

    private:
        std::vector<EnumValue> m_values;
        std::vector<const EnumValue*> m_values_ptrs;

    }; // class Enum

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT EnumValue {
    public:
        friend Enum;

        EnumValue() = default;
        EnumValue(std::string name, size_t value)
            : m_name(std::move(name)), m_value(value) {}

        const std::string& name() const { return m_name; }
        size_t value() const { return m_value; }
        const Enum* declaring_enum() const { return m_declaring_enum; }

    private:
        std::string m_name;
        size_t m_value = 0;
        const Enum* m_declaring_enum = 0;

    }; // class EnumValue

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename ENUM>
    class EnumInstance : public Enum {
    public:
        EnumInstance(std::string name, std::vector<EnumValue> values);

        bool is_default_constructible() const override;
        bool is_copy_constructible() const override;
        bool is_move_constructible() const override;
        bool is_copy_assignable() const override;
        bool is_move_assignable() const override;

        Res<Object, ErrNewObject> new_object() const override;
        Res<Object, ErrNewCopy> new_copy(const ObjectRef& src) const override;
        Res<Object, ErrNewMove> new_move(ObjectRef& src) const override;
        Res<void, ErrDeleteObject> can_delete_object(const ObjectRef& obj) const override;
        void delete_object(Object&& obj) const override;

        Res<void, ErrConstruct> can_construct(const BufferRef& buff) const override;
        ObjectRef construct(BufferRef&& buff) const override;
        Object construct(Buffer&& buff) const override;

        Res<void, ErrCopyConstruct> can_copy_construct(const BufferRef& buff
            , const ObjectRef& src) const override;
        ObjectRef copy_construct(BufferRef&& buff, const ObjectRef& src) const override;
        Object copy_construct(Buffer&& buff, const ObjectRef& src) const override;
        
        Res<void, ErrMoveConstruct> can_move_construct(const BufferRef& buff
            , const ObjectRef& src) const override;
        ObjectRef move_construct(BufferRef&& buff, ObjectRef& src) const override;
        Object move_construct(Buffer&& buff, ObjectRef& src) const override;
        
        Res<void, ErrDestruct> can_destruct(const ObjectRef& obj) const override;
        BufferRef destruct(ObjectRef&& obj) const override;
        Buffer destruct(Object&& obj) const override;
        
        Res<void, ErrCopy> copy_assign(ObjectRef& dst, const ObjectRef& src) const override;
        Res<void, ErrMove> move_assign(ObjectRef& dst, ObjectRef& src) const override;

    }; // class EnumInstance

    //*********************************************************************************************
    template <typename ENUM>
    EnumInstance<ENUM>::EnumInstance(std::string name, std::vector<EnumValue> values)
        : Enum(std::move(name), sizeof(ENUM), std::move(values)) 
    {}

    //*********************************************************************************************
    template <typename ENUM>
    bool EnumInstance<ENUM>::is_default_constructible() const {
        return std::is_default_constructible_v<ENUM>;
    }

    //*********************************************************************************************
    template <typename ENUM>
    bool EnumInstance<ENUM>::is_copy_constructible() const {
        return std::is_copy_constructible_v<ENUM>;
    }

    //*********************************************************************************************
    template <typename ENUM>
    bool EnumInstance<ENUM>::is_move_constructible() const {
        return std::is_move_constructible_v<ENUM>;
    }

    //*********************************************************************************************
    template <typename ENUM>
    bool EnumInstance<ENUM>::is_copy_assignable() const {
        return std::is_copy_assignable_v<ENUM>;
    }

    //*********************************************************************************************
    template <typename ENUM>
    bool EnumInstance<ENUM>::is_move_assignable() const {
        return std::is_move_assignable_v<ENUM>;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<Object, Type::ErrNewObject> EnumInstance<ENUM>::new_object() const {
        if (is_default_constructible())
            return Ok(Object(new ENUM()));
        else
            return Err(ErrNewObject::NOT_DEFAULT_CONSTRUCTIBLE);
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<Object, Type::ErrNewCopy> EnumInstance<ENUM>::new_copy(
        const ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewCopy::NOT_COPY_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewCopy::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(ErrNewCopy::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new ENUM(*src.value_as<ENUM>().ok())));
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<Object, Type::ErrNewMove> EnumInstance<ENUM>::new_move(
        ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewMove::NOT_MOVE_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewMove::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(ErrNewMove::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new ENUM(std::move(*src.value_as<ENUM>().ok()))));
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrDeleteObject> EnumInstance<ENUM>::can_delete_object(
        const ObjectRef& obj) const 
    {
        if (!obj.is_valid())
            return Err(ErrDeleteObject::NOT_VALID_SOURCE);
        else if (obj.type().ok() != this)
            return Err(ErrDeleteObject::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename ENUM>
    void EnumInstance<ENUM>::delete_object(Object&& obj) const {
        assert(can_delete_object(obj).is_ok());
        delete reinterpret_cast<ENUM*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrConstruct> EnumInstance<ENUM>::can_construct(
        const BufferRef& buff) const 
    {
        if (!is_default_constructible())
            return Err(Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrConstruct::BUFFER_TOO_SMALL);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename ENUM>
    ObjectRef EnumInstance<ENUM>::construct(BufferRef&& buff) const {
        assert(can_construct(buff).is_ok());
        return ObjectRef(new(buff.data().ok()) ENUM, buff.size().ok());
    }

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::construct(Buffer&& buff) const {
        assert(can_construct(buff).is_ok());
        Object res(new(buff.data().ok()) ENUM, buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrCopyConstruct> EnumInstance<ENUM>::can_copy_construct(
        const BufferRef& buff, const ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrCopyConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrCopyConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrCopyConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename ENUM>
    ObjectRef EnumInstance<ENUM>::copy_construct(BufferRef&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) ENUM(*src.value_as<ENUM>().ok()), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::copy_construct(Buffer&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) ENUM(*src.value_as<ENUM>().ok()), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrMoveConstruct> EnumInstance<ENUM>::can_move_construct(
        const BufferRef& buff, const ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE);
        else if (!buff.is_valid())
            return Err(Type::ErrMoveConstruct::INVALID_BUFFER);
        else if (buff.size().ok() < size())
            return Err(Type::ErrMoveConstruct::BUFFER_TOO_SMALL);
        else if (!src.is_valid())
            return Err(Type::ErrMoveConstruct::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename ENUM>
    ObjectRef EnumInstance<ENUM>::move_construct(BufferRef&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) 
            ENUM(std::move(*src.value_as<ENUM>().ok())), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::move_construct(Buffer&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) 
            ENUM(std::move(*src.value_as<ENUM>().ok())), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrDestruct> EnumInstance<ENUM>::can_destruct(
        const ObjectRef& obj) const 
    {
        if (!obj.is_valid())
            return Err(ErrDestruct::NOT_VALID_OBJECT);
        else if (obj.type().ok() != this)
            return Err(ErrDestruct::INCORRECT_OBJECT_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename ENUM>
    BufferRef EnumInstance<ENUM>::destruct(ObjectRef&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<ENUM*>(obj.value().ok())->~ENUM();
        BufferRef res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Buffer EnumInstance<ENUM>::destruct(Object&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<ENUM*>(obj.value().ok())->~ENUM();
        Buffer res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrCopy> EnumInstance<ENUM>::copy_assign(ObjectRef& dst
        , const ObjectRef& src) const 
    {
        if (!is_copy_assignable())
            return Err(Type::ErrCopy::NOT_COPY_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrCopy::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != this)
            return Err(Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrCopy::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != this)
            return Err(Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            *reinterpret_cast<ENUM*>(dst.value().ok()) 
                = *reinterpret_cast<const ENUM*>(src.value().ok());
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename ENUM>
    Res<void, Type::ErrMove> EnumInstance<ENUM>::move_assign(ObjectRef& dst
        , ObjectRef& src) const 
    {
        if (!is_move_assignable())
            return Err(Type::ErrMove::NOT_MOVE_ASSIGNABLE);
        else if (!dst.is_valid())
            return Err(Type::ErrMove::INVALID_DESTINATION_OBJECT);
        else if (dst.type().ok() != this)
            return Err(Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE);
        else if (!src.is_valid())
            return Err(Type::ErrMove::INVALID_SOURCE_OBJECT);
        else if (src.type().ok() != this)
            return Err(Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE);
        else {
            *reinterpret_cast<ENUM*>(dst.value().ok()) 
                = std::move(*reinterpret_cast<const ENUM*>(src.value().ok()));
            return Ok();
        }
    }
} // namespace rtti