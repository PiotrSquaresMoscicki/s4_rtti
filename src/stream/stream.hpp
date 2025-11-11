#pragma once

#include "rtti/rtti_fwd.hpp"

namespace rtti {

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

        virtual ~Stream() = default;
        Mode mode() const { return m_mode; }

        virtual Stream& operator<<(const ObjectRef& obj);
        virtual Stream& operator>>(ObjectRef& obj);

    private:
        Stream(const BufferRef& buff); // INPUT_STREAM - deserializing from the const buffer
        Stream(Buffer& buff);          // OUTPUT_STREAM - serializing into the buffer
        
        const Mode m_mode;
        const BufferRef* const m_input_buffer = nullptr;
        Buffer* const m_output_buffer = nullptr;
    }; // class Stream
} // namespace rtti