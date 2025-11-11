#include "stream.hpp"

#include "rtti/rtti.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    Stream& Stream::operator<<(const ObjectRef& obj) {
        // TODO implement
        return *this;
    }
    
    Stream& Stream::operator>>(ObjectRef& obj) {
        // TODO implement
        return *this;
    }

    Stream::Stream(const BufferRef& buff)
        : m_mode(Mode::INPUT_STREAM), m_input_buffer(&buff)
    {
    }

    Stream::Stream(Buffer& buff)
        : m_mode(Mode::OUTPUT_STREAM), m_output_buffer(&buff)
    {
    }



} // namespace rtti