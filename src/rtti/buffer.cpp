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
Res<const void*, BufferRef::ErrData> BufferRef::data() const { 
    if (is_valid()) 
        return Ok(const_cast<const void*>(m_data)); 
    else 
        return Err(ErrData::INVALID_BUFFER);
}

//*************************************************************************************************
Res<void*, BufferRef::ErrData> BufferRef::data() { 
    if (is_valid()) 
        return Ok(m_data); 
    else 
        return Err(ErrData::INVALID_BUFFER);
}

//*************************************************************************************************
Res<void*, BufferRef::ErrData> BufferRef::steal_data() && {
    if (!is_valid())
        return Err(ErrData::INVALID_BUFFER);

    void* res = m_data;
    m_data = nullptr;
    m_size = 0;
    return Ok(res);
}

//*************************************************************************************************
void BufferRef::copy(const BufferRef&) {
    
}

//*************************************************************************************************
Res<size_t, BufferRef::ErrSize> BufferRef::size() const { 
    if (is_valid()) 
        return Ok(m_size); 
    else 
        return Err(ErrSize::INVALID_BUFFER);
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
Object Buffer::call_constructor(const Type*) && {
    return {};
}