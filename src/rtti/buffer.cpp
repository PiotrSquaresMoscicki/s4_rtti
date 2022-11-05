#include "buffer.hpp"

#include "object.hpp"

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
BufferRef::BufferRef(void* data, size_t size)
    : m_data(data)
    , m_size(size)
{}

//*************************************************************************************************
BufferRef::BufferRef(BufferRef&& other)
    : m_data(other.m_data)
    , m_size(other.m_size)
{
    other.m_data = nullptr;
    other.m_size = 0;
}

//*************************************************************************************************
BufferRef& BufferRef::operator=(BufferRef&& other) {
    m_data = other.m_data;
    m_size = other.m_size;
    other.m_data = nullptr;
    other.m_size = 0;
    return *this;
}

//*************************************************************************************************
Res<const void*, BufferRef::Error> BufferRef::data() const { 
    if (is_valid()) 
        return Ok(const_cast<const void*>(m_data)); 
    else 
        return Err(Error::INVALID_BUFFER);
}

//*************************************************************************************************
Res<void*, BufferRef::Error> BufferRef::data() { 
    if (is_valid()) 
        return Ok(m_data); 
    else 
        return Err(Error::INVALID_BUFFER);
}

//*************************************************************************************************
Res<void*, BufferRef::Error> BufferRef::steal_data() {
    if (!is_valid())
        return Err(Error::INVALID_BUFFER);

    void* res = m_data;
    m_data = nullptr;
    m_size = 0;
    return Ok(res);
}

//*************************************************************************************************
Res<size_t, BufferRef::Error> BufferRef::size() const { 
    if (is_valid()) 
        return Ok(m_size); 
    else 
        return Err(Error::INVALID_BUFFER);
}

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
Buffer::Buffer(size_t size)
    : BufferRef(new char[size], size)
{}

//*************************************************************************************************
Buffer::~Buffer() {
    if (is_valid())
        delete[] reinterpret_cast<char*>(m_data);
}

//*************************************************************************************************
void Buffer::change_buffer_size(size_t) {
}

//*************************************************************************************************
void Buffer::release_buffer() && {
}

//*************************************************************************************************
Object Buffer::call_constructor(TypePtr) && {
    return {};
}