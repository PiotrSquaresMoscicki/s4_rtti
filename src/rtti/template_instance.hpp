#pragma once

#include "class.hpp"

namespace rtti {

    class TemplateParam;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT TemplateInstance : public Class {
    public:
        TemplateInstance(std::string name, size_t size, Attributes attributes
            , std::vector<TemplateParam> params);

        bool is_template_instance() const override { return true; }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Ok(TemplateInstancePtr(this)); 
        }
    
        const std::vector<const TemplateParam*>& params() const { return m_params_ptrs; }

    protected:
        std::vector<TemplateParam> m_params;
        std::vector<const TemplateParam*> m_params_ptrs;

    }; // class TemplateInstance
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class TemplateParam {
    public:
        friend class TemplateInstance;

        TemplateParam(std::string name, TypePtr type) 
            : m_name(std::move(name)), m_type(type) {}

        const std::string& name() const { return m_name; }
        TypePtr type() const { return m_type; }
        const TemplateInstance* declaring_template_instance() const 
            { return m_declaring_template_instance; }

    private:
        std::string m_name;
        TypePtr m_type = nullptr;
        const TemplateInstance* m_declaring_template_instance = nullptr;
    };

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    class TemplateInstanceInstance : public TemplateInstance {
    public:
        TemplateInstanceInstance(const std::string& name
            , std::vector<std::string>& out_params_names);
        TemplateInstanceInstance(const std::string& name
            , std::vector<std::string>& out_params_names, Attributes attributes);

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

        static std::string generate_name(const std::string&
            , std::vector<std::string>&) 
        {
            /*bool opening_bracket_found = false;
            bool closing_bracket_found = false;
            std::string curr_name;

            for (size_t i = 0; i < name.length(); ++i) {
                if (name[i] == ' ' && name[i + 1] == '<') {
                    assert(opening_bracket_found || closing_bracket_found == false);
                    out_params_names.push_back(std::move(curr_name));
                    opening_bracket_found = true;
                    ++i;
                } else if (name[i] == ' ') {
                    assert(opening_bracket_found == true && closing_bracket_found == false);
                    out_params_names.push_back(std::move(curr_name));
                } else if (name[i] == '>') {
                    assert(opening_bracket_found == true && closing_bracket_found == false);
                    out_params_names.push_back(std::move(curr_name));
                    closing_bracket_found = true;
                } else
                    curr_name += name[i];
            }

            std::string result = out_params_names.front() + "<" + generate_params_for_name<>() + ">";
            out_params_names.erase(out_params_names.begin());
            return result;*/
            return "";
        }

/*        template <size_t IDX = std::tuple_size<typename DECLARING_CLASS::ParamsTuple>::value>
        static std::string generate_params_for_name() {
            std::string result = generate_params_for_name<IDX - 1>();

            std::string next_param
                = static_type<typename std::tuple_element_t<IDX, typename DECLARING_CLASS::ParamsTuple>>()
                    ->name();

                result = result + ", " + next_param;

            return result;
        }

        template<>
        static std::string generate_params_for_name<0>() {
            return static_type<
                    typename std::tuple_element_t<0, typename DECLARING_CLASS::ParamsTuple>
                >()->name();
        }*/

        template <size_t IDX>
        static std::vector<TemplateParam> generate_params(
            const std::vector<std::string>&) 
        {
            //assert(params_names.size() == 1);
            //TemplateParam param(params_names[IDX]
            //    , static_type<typename std::tuple_element<IDX, typename DECLARING_CLASS::ParamsTuple>::type>());
            std::vector<TemplateParam> result;
            //result.push_back(std::move(param));

            // if constexpr (IDX < std::tuple_size<typename DECLARING_CLASS::ParamsTuple>::value) {
            //     std::vector<TemplateParam> next_params = generate_params<IDX + 1>(params_names);
            //     result.insert(result.end(), next_params.begin(), next_params.end());
            // }

            return result;
        }
        
    }; // class TemplateInstanceInstance

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    TemplateInstanceInstance<CLASS, DECLARING_CLASS>::TemplateInstanceInstance(
        const std::string& name, std::vector<std::string>& out_params_names)
        : TemplateInstanceInstance(name, out_params_names, {}) 
    {}

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    TemplateInstanceInstance<CLASS, DECLARING_CLASS>::TemplateInstanceInstance(
        const std::string& name, std::vector<std::string>& out_params_names, Attributes attributes)
        : TemplateInstance(generate_name(name, out_params_names), sizeof(CLASS)
        , std::move(attributes), generate_params<0>(out_params_names)) 
    {}

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    bool TemplateInstanceInstance<CLASS, DECLARING_CLASS>::is_default_constructible() const {
        return std::is_default_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    bool TemplateInstanceInstance<CLASS, DECLARING_CLASS>::is_copy_constructible() const {
        return std::is_copy_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    bool TemplateInstanceInstance<CLASS, DECLARING_CLASS>::is_move_constructible() const {
        return std::is_move_constructible_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    bool TemplateInstanceInstance<CLASS, DECLARING_CLASS>::is_copy_assignable() const {
        return std::is_copy_assignable_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    bool TemplateInstanceInstance<CLASS, DECLARING_CLASS>::is_move_assignable() const {
        return std::is_move_assignable_v<CLASS>;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<Object, Type::ErrNewObject> TemplateInstanceInstance<CLASS, DECLARING_CLASS>
        ::new_object() const 
    {
        if (is_default_constructible())
            return Ok(Object(new CLASS()));
        else
            return Err(ErrNewObject::NOT_DEFAULT_CONSTRUCTIBLE);
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<Object, Type::ErrNewCopy> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::new_copy(
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
    template <typename CLASS, typename DECLARING_CLASS>
    Res<Object, Type::ErrNewMove> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::new_move(
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
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrDeleteObject> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::can_delete_object(
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
    template <typename CLASS, typename DECLARING_CLASS>
    void TemplateInstanceInstance<CLASS, DECLARING_CLASS>::delete_object(Object&& obj) const {
        assert(can_delete_object(obj).is_ok());
        delete reinterpret_cast<CLASS*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrConstruct> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::can_construct(
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
    template <typename CLASS, typename DECLARING_CLASS>
    ObjectRef TemplateInstanceInstance<CLASS, DECLARING_CLASS>::construct(BufferRef&& buff) const {
        assert(can_construct(buff).is_ok());
        return ObjectRef(new(buff.data().ok()) CLASS, buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Object TemplateInstanceInstance<CLASS, DECLARING_CLASS>::construct(Buffer&& buff) const {
        assert(can_construct(buff).is_ok());
        Object res(new(buff.data().ok()) CLASS, buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrCopyConstruct> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::can_copy_construct(
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
    template <typename CLASS, typename DECLARING_CLASS>
    ObjectRef TemplateInstanceInstance<CLASS, DECLARING_CLASS>::copy_construct(BufferRef&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) CLASS(*src.value_as<CLASS>().ok()), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Object TemplateInstanceInstance<CLASS, DECLARING_CLASS>::copy_construct(Buffer&& buff
        , const ObjectRef& src) const 
    {
        assert(can_copy_construct(buff, src).is_ok());
        Object res(
            new(buff.data().ok()) CLASS(*src.value_as<CLASS>().ok()), buff.size().ok());
        std::move(buff).steal_data();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrMoveConstruct> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::can_move_construct(
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
    template <typename CLASS, typename DECLARING_CLASS>
    ObjectRef TemplateInstanceInstance<CLASS, DECLARING_CLASS>::move_construct(BufferRef&& buff
        , ObjectRef& src) const 
    {
        assert(can_move_construct(buff, src).is_ok());
        return ObjectRef(
            new(buff.data().ok()) 
            CLASS(std::move(*src.value_as<CLASS>().ok())), buff.size().ok());
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Object TemplateInstanceInstance<CLASS, DECLARING_CLASS>::move_construct(Buffer&& buff
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
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrDestruct> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::can_destruct(
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
    template <typename CLASS, typename DECLARING_CLASS>
    BufferRef TemplateInstanceInstance<CLASS, DECLARING_CLASS>::destruct(ObjectRef&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<CLASS*>(obj.value().ok())->~CLASS();
        BufferRef res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Buffer TemplateInstanceInstance<CLASS, DECLARING_CLASS>::destruct(Object&& obj) const {
        assert(can_destruct(obj).is_ok());
        reinterpret_cast<CLASS*>(obj.value().ok())->~CLASS();
        Buffer res(obj.value().ok(), obj.size().ok());
        std::move(obj).steal_value();
        return res;
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrCopy> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::copy_assign(ObjectRef& dst
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
    template <typename CLASS, typename DECLARING_CLASS>
    Res<void, Type::ErrMove> TemplateInstanceInstance<CLASS, DECLARING_CLASS>::move_assign(ObjectRef& dst
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

} // namespace rtti

