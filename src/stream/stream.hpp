#pragma once

#include <memory>

#include "rtti/rtti_fwd.hpp"
#include "rtti/buffer.hpp"

namespace rtti {
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT BufferInputStream {
    public: 
        BufferInputStream(const BufferRef& buff);

        BufferInputStream& operator>>(BufferRef& buff_ref);
        BufferInputStream& operator>>(ObjectRef& obj_ref);

    private:
        const BufferRef& m_buffer;
        size_t m_read_offset = 0;
    };

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT BufferOutputStream {
    public:
        BufferOutputStream();

        BufferOutputStream& operator<<(const BufferRef& buff_ref);
        BufferOutputStream& operator<<(const ObjectRef& obj_ref);

    private:
        Buffer m_buffer;
        size_t m_write_offset = 0;
    };

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Stream {
    public: 
        // this class provides << and >> operators for ObjectRef to serialize into the stream
        // it is supposed to be generic way to go through all members of an object recursively
        // derived classes can do with this information whatever they want. The most obvious use case
        // is to serialize the object into some binary or text format. Another use case is to 
        // calculate some kind of hash or checksum of the object by going through all its members.
        // And another one is to implement a diff/patch mechanism by comparing two objects of the same type.
        // Anmd another is to implement a garbage collection functionality by performing a reachability analysis
        // starting from a set of root objects, marking all reachable objects and deleting the unmarked ones.
        // For the ease of use the Stream object contains information if its serializing or deserializing data
        // so that the user can just use the same code for both operations.
        // >> operator can perform both operations - input and output - depending on the mode of the stream.
        // << operator is hete to avoid const casts in case it is used only as output stream.

        enum class Mode {
            INPUT_STREAM, // deserializing - data flowing from the stream into the object
            OUTPUT_STREAM // serializing - data flowing from the object into the stream
        };

        Stream() = default;
        virtual ~Stream() = default;
        Mode mode() const { return m_mode; }

        virtual Stream& operator<<(const ObjectRef& obj) = 0;
        virtual Stream& operator>>(ObjectRef& obj) = 0;

    private:
        const Mode m_mode;
    }; // class Stream

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class SerializationStream : public Stream {
    public:
        // INPUT_STREAM - deserializing from the const buffer
        SerializationStream(std::unique_ptr<BufferInputStream> input_stream);

        // OUTPUT_STREAM - serializing into the buffer
        SerializationStream(std::unique_ptr<BufferOutputStream> output_stream);

        virtual ~SerializationStream() = default;
        
        virtual Stream& operator<<(const ObjectRef& obj) override;
        virtual Stream& operator>>(ObjectRef& obj) override;

        std::unique_ptr<BufferOutputStream> steal_output_stream() &&;

    private:
        std::unique_ptr<BufferInputStream> m_input_stream;
        std::unique_ptr<BufferOutputStream> m_output_stream;
    }; // class SerializationStream

} // namespace rtti