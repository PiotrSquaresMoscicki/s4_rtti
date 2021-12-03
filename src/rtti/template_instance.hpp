#pragma once

#include "class.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT TemplateInstance : public Class {
    public:
        TemplateInstance(std::string name, size_t size, Attributes attributes) 
            : Class(std::move(name), size, std::move(attributes)) {}

        const TemplateInstance* as_template_instance() const override { return this; }

    }; // class TemplateInstance

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS>
    class S4_RTTI_EXPORT TemplateInstanceInstance : public TemplateInstance {
    public:
        TemplateInstanceInstance(const std::string& name);
        TemplateInstanceInstance(const std::string& name, Attributes attributes);

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

    private:
        static std::string generate_name(const std::string& name);

    }; // class TemplateInstanceInstance

    //*********************************************************************************************
    template <typename CLASS>
    TemplateInstanceInstance<CLASS>::TemplateInstanceInstance(const std::string& name)
        : TemplateInstance(generate_name(name), sizeof(CLASS), {}) 
    {}

    //*********************************************************************************************
    template <typename CLASS>
    TemplateInstanceInstance<CLASS>::TemplateInstanceInstance(const std::string& name
        , Attributes attributes)
        : TemplateInstance(generate_name(name), sizeof(CLASS), std::move(attributes)) 
    {}

    //*********************************************************************************************
    template <typename CLASS>
    bool TemplateInstanceInstance<CLASS>::is_default_constructible() const {
        return std::is_default_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS>
    bool TemplateInstanceInstance<CLASS>::is_copy_constructible() const {
        return std::is_copy_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS>
    bool TemplateInstanceInstance<CLASS>::is_move_constructible() const {
        return std::is_move_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS>
    bool TemplateInstanceInstance<CLASS>::is_copy_assignable() const {
        return std::is_copy_assignable_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS>
    bool TemplateInstanceInstance<CLASS>::is_move_assignable() const {
        return std::is_move_assignable_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<Object, Type::ErrNewObject> TemplateInstanceInstance<CLASS>::new_object() const {
        if (is_default_constructible())
            return Ok(Object(new CLASS()));
        else
            return Err(ErrNewObject::NOT_DEFAULT_CONSTRUCTIBLE);
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<Object, Type::ErrNewCopy> TemplateInstanceInstance<CLASS>::new_copy(
        const ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewCopy::NOT_COPY_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewCopy::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(ErrNewCopy::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new CLASS(*src.value_as<CLASS>().ok())));
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<Object, Type::ErrNewMove> TemplateInstanceInstance<CLASS>::new_move(
        ObjectRef& src) const 
    {
        if (!is_copy_constructible())
            return Err(ErrNewMove::NOT_MOVE_CONSTRUCTIBLE);
        else if (!src.is_valid())
            return Err(ErrNewMove::NOT_VALID_SOURCE);
        else if (src.type().ok() != this)
            return Err(ErrNewMove::INCORRECT_SOURCE_TYPE);
        else
            return Ok(Object(new CLASS(std::move(*src.value_as<CLASS>().ok()))));
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrDeleteObject> TemplateInstanceInstance<CLASS>::can_delete_object(
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
    template <typename CLASS>
    void TemplateInstanceInstance<CLASS>::delete_object(Object&& obj) const {
        assert(can_delete_object(obj).is_ok());
        delete reinterpret_cast<CLASS*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrConstruct> TemplateInstanceInstance<CLASS>::can_construct(
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
    template <typename CLASS>
    ObjectRef TemplateInstanceInstance<CLASS>::construct(BufferRef&& buff) const {
        assert(can_construct(buff).is_ok());
        return ObjectRef(new(buff.data().ok()) CLASS, buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS>
    Object TemplateInstanceInstance<CLASS>::construct(Buffer&& buff) const {
        assert(can_construct(buff).is_ok());
        Object res(new(buff.data().ok()) CLASS, buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrCopyConstruct> TemplateInstanceInstance<CLASS>::can_copy_construct(
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
    template <typename CLASS>
    ObjectRef TemplateInstanceInstance<CLASS>::copy_construct(BufferRef&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) CLASS(*src.value_as<CLASS>().ok()), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS>
    Object TemplateInstanceInstance<CLASS>::copy_construct(Buffer&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) CLASS(*src.value_as<CLASS>().ok()), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrMoveConstruct> TemplateInstanceInstance<CLASS>::can_move_construct(
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
    template <typename CLASS>
    ObjectRef TemplateInstanceInstance<CLASS>::move_construct(BufferRef&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) 
            CLASS(std::move(*src.value_as<CLASS>().ok())), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS>
    Object TemplateInstanceInstance<CLASS>::move_construct(Buffer&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) 
            CLASS(std::move(*src.value_as<CLASS>().ok())), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrDestruct> TemplateInstanceInstance<CLASS>::can_destruct(
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
    template <typename CLASS>
    BufferRef TemplateInstanceInstance<CLASS>::destruct(ObjectRef&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<CLASS*>(obj.value().ok())->~CLASS();
        BufferRef res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Buffer TemplateInstanceInstance<CLASS>::destruct(Object&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<CLASS*>(obj.value().ok())->~CLASS();
        Buffer res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrCopy> TemplateInstanceInstance<CLASS>::copy_assign(ObjectRef& dst
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
            *reinterpret_cast<CLASS*>(dst.value().ok()) 
                = *reinterpret_cast<const CLASS*>(src.value().ok());
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename CLASS>
    Res<void, Type::ErrMove> TemplateInstanceInstance<CLASS>::move_assign(ObjectRef& dst
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
            *reinterpret_cast<CLASS*>(dst.value().ok()) 
                = std::move(*reinterpret_cast<const CLASS*>(src.value().ok()));
            return Ok();
        }
    }

    //*********************************************************************************************
    template <typename CLASS>
    std::string TemplateInstanceInstance<CLASS>::generate_name(const std::string& name) {
        size_t beg = 0;
        size_t end = name.find('<', beg);
        std::string template_name = name.substr(beg, end);
        beg = end;
        std::vector<std::string> params_names;

        while (end != std::string::npos) {
            end = name.find(' ', beg);
            
        }
    }

} // namespace rtti