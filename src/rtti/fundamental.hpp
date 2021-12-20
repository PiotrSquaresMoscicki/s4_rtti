#pragma once

#include <cassert>
#include <type_traits>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Fundamental : public Type {
    public:
        Fundamental(std::string name, size_t size);

        bool is_fundamental() const override { return true; }
        bool is_enum() const override { return false; }
        bool is_class() const override { return false; }
        bool is_template_instance() const override { return false; }

        Res<FundamentalPtr, ErrAsFundamental> as_fundamental() const override { 
            return Ok(FundamentalPtr(this)); 
        }
        Res<EnumPtr, ErrAsEnum> as_enum() const override { 
            return Err(ErrAsEnum::NOT_AN_ENUM); 
        }
        Res<ClassPtr, ErrAsClass> as_class() const override { 
            return Err(ErrAsClass::NOT_A_CLASS); 
        }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Err(ErrAsTemplateInstance::NOT_A_TEMPLATE_INSTANCE); 
        }

    }; // class Fundamental

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    class FundamentalInstance : public Fundamental {
    public:
        FundamentalInstance(std::string name);

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

    }; // class FundamentalInstance

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    FundamentalInstance<FUNDAMENTAL>::FundamentalInstance(std::string name)
        : Fundamental(std::move(name), sizeof(FUNDAMENTAL)) 
    {}

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    bool FundamentalInstance<FUNDAMENTAL>::is_default_constructible() const {
        return std::is_default_constructible_v<FUNDAMENTAL>;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    bool FundamentalInstance<FUNDAMENTAL>::is_copy_constructible() const {
        return std::is_copy_constructible_v<FUNDAMENTAL>;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    bool FundamentalInstance<FUNDAMENTAL>::is_move_constructible() const {
        return std::is_move_constructible_v<FUNDAMENTAL>;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    bool FundamentalInstance<FUNDAMENTAL>::is_copy_assignable() const {
        return std::is_copy_assignable_v<FUNDAMENTAL>;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    bool FundamentalInstance<FUNDAMENTAL>::is_move_assignable() const {
        return std::is_move_assignable_v<FUNDAMENTAL>;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<Object, Type::ErrNewObject> FundamentalInstance<FUNDAMENTAL>::new_object() const {
        if (is_default_constructible())
            return Ok(Object(new FUNDAMENTAL()));
        else
            return Err(ErrNewObject::NOT_DEFAULT_CONSTRUCTIBLE);
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<Object, Type::ErrNewCopy> FundamentalInstance<FUNDAMENTAL>::new_copy(
        const ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewCopy::NOT_COPY_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewCopy::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(ErrNewCopy::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new FUNDAMENTAL(*src.value_as<FUNDAMENTAL>().ok())));
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<Object, Type::ErrNewMove> FundamentalInstance<FUNDAMENTAL>::new_move(
        ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewMove::NOT_MOVE_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewMove::NOT_VALID_SOURCE);
        else if (src.type().ok() != TypePtr(this))
            return Err(ErrNewMove::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new FUNDAMENTAL(std::move(*src.value_as<FUNDAMENTAL>().ok()))));
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrDeleteObject> FundamentalInstance<FUNDAMENTAL>::can_delete_object(
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
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::delete_object(Object&& obj) const {
        assert(can_delete_object(obj).is_ok());
        delete reinterpret_cast<FUNDAMENTAL*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrConstruct> FundamentalInstance<FUNDAMENTAL>::can_construct(
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
    template <typename FUNDAMENTAL>
    ObjectRef FundamentalInstance<FUNDAMENTAL>::construct(BufferRef&& buff) const {
        assert(can_construct(buff).is_ok());
        return ObjectRef(new(buff.data().ok()) FUNDAMENTAL, buff.size().ok());
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::construct(Buffer&& buff) const {
        assert(can_construct(buff).is_ok());
        Object res(new(buff.data().ok()) FUNDAMENTAL, buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrCopyConstruct> FundamentalInstance<FUNDAMENTAL>::can_copy_construct(
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
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    ObjectRef FundamentalInstance<FUNDAMENTAL>::copy_construct(BufferRef&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) FUNDAMENTAL(*src.value_as<FUNDAMENTAL>().ok()), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::copy_construct(Buffer&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) FUNDAMENTAL(*src.value_as<FUNDAMENTAL>().ok()), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrMoveConstruct> FundamentalInstance<FUNDAMENTAL>::can_move_construct(
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
        else if (src.type().ok() != TypePtr(this))
            return Err(Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE);
        else
            return Ok();
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    ObjectRef FundamentalInstance<FUNDAMENTAL>::move_construct(BufferRef&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) 
            FUNDAMENTAL(std::move(*src.value_as<FUNDAMENTAL>().ok())), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::move_construct(Buffer&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) 
            FUNDAMENTAL(std::move(*src.value_as<FUNDAMENTAL>().ok())), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrDestruct> FundamentalInstance<FUNDAMENTAL>::can_destruct(
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
    template <typename FUNDAMENTAL>
    BufferRef FundamentalInstance<FUNDAMENTAL>::destruct(ObjectRef&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<FUNDAMENTAL*>(obj.value().ok())->~FUNDAMENTAL();
        BufferRef res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Buffer FundamentalInstance<FUNDAMENTAL>::destruct(Object&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<FUNDAMENTAL*>(obj.value().ok())->~FUNDAMENTAL();
        Buffer res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrCopy> FundamentalInstance<FUNDAMENTAL>::copy_assign(ObjectRef& dst
        , const ObjectRef& src) const 
    {
        if (!is_copy_assignable())
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
            *reinterpret_cast<FUNDAMENTAL*>(dst.value().ok()) 
                = *reinterpret_cast<const FUNDAMENTAL*>(src.value().ok());
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Res<void, Type::ErrMove> FundamentalInstance<FUNDAMENTAL>::move_assign(ObjectRef& dst
        , ObjectRef& src) const 
    {
        if (!is_move_assignable())
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
            *reinterpret_cast<FUNDAMENTAL*>(dst.value().ok()) 
                = std::move(*reinterpret_cast<const FUNDAMENTAL*>(src.value().ok()));
            return Ok();
        }
    }

} // namespace rtti