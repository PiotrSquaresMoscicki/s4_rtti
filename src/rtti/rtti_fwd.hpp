#pragma once

#include "src/s4_rtti_export.h"

namespace rtti {

    class Type;
    class Enum;
    class Fundamental;
    class Class;
    class TemplateInstance;

    class ObjectRef;
    class Object;
    class BufferRef;
    class Buffer;

    class Member;
    class Field;
    class Property;
    class Method;

    class Attributes;

    class FundamentalPtr;
    class EnumPtr;
    class ClassPtr;
    class TemplateInstancePtr;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class TypePtr {
    public:
        TypePtr() = default;
        TypePtr(const Type* type) : m_type(type) {}
        TypePtr(const TypePtr& other) : m_type(other.m_type) {}
        TypePtr& operator=(const TypePtr& other) { m_type = other.m_type; return *this; }
        
        bool operator==(const TypePtr& other) const { return m_type == other.m_type; }
        bool operator!=(const TypePtr& other) const { return m_type != other.m_type; }
        bool operator<(const TypePtr& other) const { return m_type < other.m_type; }
        bool operator<=(const TypePtr& other) const { return m_type <= other.m_type; }
        bool operator>(const TypePtr& other) const { return m_type > other.m_type; }
        bool operator>=(const TypePtr& other) const { return m_type >= other.m_type; }
        const Type* operator->() const { return m_type; }
        operator bool() const { return is_valid(); }

        bool is_valid() const { return m_type; }
        const Type* get() const { return m_type; }

    private:
        const Type* m_type = nullptr;

    }; // class TypePtr
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class FundamentalPtr {
    public:
        FundamentalPtr() = default;
        FundamentalPtr(const Fundamental* type) : m_type(type) {}
        FundamentalPtr(const FundamentalPtr& other) : m_type(other.m_type) {}
        FundamentalPtr& operator=(const FundamentalPtr& other) { m_type = other.m_type; return *this; }
        
        bool operator==(const FundamentalPtr& other) const { return m_type == other.m_type; }
        bool operator!=(const FundamentalPtr& other) const { return m_type != other.m_type; }
        bool operator<(const FundamentalPtr& other) const { return m_type < other.m_type; }
        bool operator>(const FundamentalPtr& other) const { return m_type > other.m_type; }
        bool operator>=(const FundamentalPtr& other) const { return m_type >= other.m_type; }
        const Fundamental* operator->() const { return m_type; }
        operator bool() const { return is_valid(); }
        operator TypePtr() const;

        bool is_valid() const { return m_type; }
        const Fundamental* get() const { return m_type; }

    private:
        const Fundamental* m_type = nullptr;

    }; // class FundamentalPtr
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class EnumPtr {
    public:
        EnumPtr() = default;
        EnumPtr(const Enum* type) : m_type(type) {}
        EnumPtr(const EnumPtr& other) : m_type(other.m_type) {}
        EnumPtr& operator=(const EnumPtr& other) { m_type = other.m_type; return *this;}
        
        bool operator==(const EnumPtr& other) const { return m_type == other.m_type; }
        bool operator!=(const EnumPtr& other) const { return m_type != other.m_type; }
        bool operator<(const EnumPtr& other) const { return m_type < other.m_type; }
        bool operator>(const EnumPtr& other) const { return m_type > other.m_type; }
        bool operator>=(const EnumPtr& other) const { return m_type >= other.m_type; }
        const Enum* operator->() const { return m_type; }
        operator bool() const { return is_valid(); }
        operator TypePtr() const;

        bool is_valid() const { return m_type; }
        const Enum* get() const { return m_type; }

    private:
        const Enum* m_type = nullptr;

    }; // class EnumPtr
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ClassPtr {
    public:
        ClassPtr() = default;
        ClassPtr(const Class* type) : m_type(type) {}
        ClassPtr(const ClassPtr& other) : m_type(other.m_type) {}
        ClassPtr& operator=(const ClassPtr& other) { m_type = other.m_type; return *this; }
        
        bool operator==(const ClassPtr& other) const { return m_type == other.m_type; }
        bool operator!=(const ClassPtr& other) const { return m_type != other.m_type; }
        bool operator<(const ClassPtr& other) const { return m_type < other.m_type; }
        bool operator>(const ClassPtr& other) const { return m_type > other.m_type; }
        bool operator>=(const ClassPtr& other) const { return m_type >= other.m_type; }
        const Class* operator->() const { return m_type; }
        operator bool() const { return is_valid(); }
        operator TypePtr() const;

        bool is_valid() const { return m_type; }
        const Class* get() const { return m_type; }

    private:
        const Class* m_type = nullptr;

    }; // class ClassPtr
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class TemplateInstancePtr {
    public:
        TemplateInstancePtr() = default;
        TemplateInstancePtr(const TemplateInstance* type) : m_type(type) {}
        TemplateInstancePtr(const TemplateInstancePtr& other) : m_type(other.m_type) {}
        TemplateInstancePtr& operator=(const TemplateInstancePtr& other) { m_type = other.m_type; return *this;}
        
        bool operator==(const TemplateInstancePtr& other) const { return m_type == other.m_type; }
        bool operator!=(const TemplateInstancePtr& other) const { return m_type != other.m_type; }
        bool operator<(const TemplateInstancePtr& other) const { return m_type < other.m_type; }
        bool operator>(const TemplateInstancePtr& other) const { return m_type > other.m_type; }
        bool operator>=(const TemplateInstancePtr& other) const { return m_type >= other.m_type; }
        const TemplateInstance* operator->() const { return m_type; }
        operator bool() const { return is_valid(); }
        operator TypePtr() const;
        operator ClassPtr() const;

        bool is_valid() const { return m_type; }
        const TemplateInstance* get() const { return m_type; }

    private:
        const TemplateInstance* m_type = nullptr;

    }; // class TemplateInstancePtr

    template <typename TYPE>
    inline TypePtr static_type();

    template <typename CLASS>
    inline ClassPtr static_class();
    
    template <typename CLASS>
    inline ClassPtr dynamic_class(CLASS& obj);

} // namespace rtti

#define STR(ARG) #ARG
#define COMMA ,