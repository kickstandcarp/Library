#ifndef OPENGL_BUFFER_HPP
#define OPENGL_BUFFER_HPP

#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include "opengl/buffer_support.hpp"

class Buffer
{
    public:
        Buffer();
        ~Buffer();

        unsigned int                        get_id() const;
        unsigned int                        get_size() const;
        GLenum                              get_type() const;
        unsigned int                        get_dimension() const;
        template <class T> T                get(const unsigned int offset, const bool as_bytes) const;
        template <class T> std::vector<T>   get(const unsigned int offset, const unsigned int size, const bool as_bytes) const;
        
        template <class T> void             set(const T &data, const unsigned int offset, const bool as_bytes);
        template <class T> void             set(const std::vector<T> &data, const unsigned int offset, const bool as_bytes);

        template <class T> void             initialize(const std::vector<T> &data, const BufferUsageFrequency frequency=BufferUsageFrequency::statical, const BufferUsageAccess access=BufferUsageAccess::draw);

    private:
        unsigned int                        id, size, dimension;

        GLenum                              type;
};



template <class T>
T Buffer::get(const unsigned int offset, const bool as_bytes) const
{
    if (offset > this->size)
        throw std::length_error("buffer length error");

    T data;
    glBindBuffer(GL_COPY_READ_BUFFER, this->id);
    glGetBufferSubData(GL_COPY_READ_BUFFER, offset*(as_bytes ? 1 : sizeof(T)), sizeof(T), &data);
    glBindBuffer(GL_COPY_READ_BUFFER, 0);

    return data;
}
template <class T>
std::vector<T> Buffer::get(const unsigned int offset, const unsigned int size, const bool as_bytes) const
{
    if (offset + size > this->size)
        throw std::length_error("buffer length error");

    std::vector<T> data(size);
    glBindBuffer(GL_COPY_READ_BUFFER, this->id);
    glGetBufferSubData(GL_COPY_READ_BUFFER, offset*(as_bytes ? 1 : sizeof(T)), size*sizeof(T), data.data());
    glBindBuffer(GL_COPY_READ_BUFFER, 0);

    return data;
}

template <class T>
void Buffer::set(const T &data, const unsigned int offset, const bool as_bytes)
{
    if (offset > this->size)
        throw std::length_error("buffer length error");

    glBindBuffer(GL_COPY_WRITE_BUFFER, this->id);
    glBufferSubData(GL_COPY_WRITE_BUFFER, offset*(as_bytes ? 1 : sizeof(T)), sizeof(T), &data);
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

template <class T>
void Buffer::set(const std::vector<T> &data, const unsigned int offset, const bool as_bytes)
{
    if (offset + data.size() > this->size)
        throw std::length_error("buffer length error");

    glBindBuffer(GL_COPY_WRITE_BUFFER, this->id);
    glBufferSubData(GL_COPY_WRITE_BUFFER, offset*(as_bytes ? 1 : sizeof(T)), data.size()*sizeof(T), data.data());
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

template <class T>
void Buffer::initialize(const std::vector<T> &data, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    this->size = data.size();
    this->dimension = buffer_dimension<T>();
    this->type = buffer_type<T>();

    glBindBuffer(GL_COPY_WRITE_BUFFER, this->id);
    glBufferData(GL_COPY_WRITE_BUFFER, data.size()*sizeof(T), data.data(), buffer_usage(frequency, access));
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

#endif
