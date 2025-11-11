#include "stream.hpp"

#include "rtti/rtti.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    BufferInputStream::BufferInputStream(const BufferRef& buff)
        : m_buffer(buff)
    {
    }

    BufferInputStream& BufferInputStream::operator>>(BufferRef& buff_ref) {
        // TODO implement buffer reading logic
        return *this;
    }

    BufferInputStream& BufferInputStream::operator>>(ObjectRef& obj_ref) {
        // TODO implement object reading logic
        return *this;
    }
    
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    BufferOutputStream::BufferOutputStream()
    {
    }

    BufferOutputStream& BufferOutputStream::operator<<(const BufferRef& buff_ref) {
        // TODO implement buffer writing logic
        return *this;
    }

    BufferOutputStream& BufferOutputStream::operator<<(const ObjectRef& obj_ref) {
        // TODO implement object writing logic
        return *this;
    }

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    Stream& SerializationStream::operator<<(const ObjectRef& obj) {
        assert(mode() == Mode::OUTPUT_STREAM);

        if (auto as_fundamental = obj.type().ok()->as_fundamental(); as_fundamental.is_ok()) {
            // TODO implement fundamental serialization
        }
        else if (auto as_enum = obj.type().ok()->as_enum(); as_enum.is_ok()) {
            // TODO implement enum serialization
        }
        else if (auto as_class = obj.type().ok()->as_class(); as_class.is_ok()) {
            // TODO implement class serialization
        }
        else {
            // TODO handle other types or error
        }
        
        return *this;
    }
    
    Stream& SerializationStream::operator>>(ObjectRef& obj) {
        if (mode() == Mode::OUTPUT_STREAM) {
            return this->operator<<(obj);
        }
        else {
            if (auto as_fundamental = obj.type().ok()->as_fundamental(); as_fundamental.is_ok()) {
                // TODO implement fundamental deserialization
            }
            else if (auto as_enum = obj.type().ok()->as_enum(); as_enum.is_ok()) {
                // TODO implement enum deserialization
            }
            else if (auto as_class = obj.type().ok()->as_class(); as_class.is_ok()) {
                // TODO implement class deserialization
            }
            else {
                // TODO handle other types or error
            }

            return *this;
        }
    }



} // namespace rtti