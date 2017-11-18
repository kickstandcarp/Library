#include <stdexcept>

#include <glm/glm.hpp>

#include "opengl/buffer_support.hpp"



GLenum buffer_usage(const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    switch (frequency)
    {
        case BufferUsageFrequency::stream:
            switch (access)
            {
                case BufferUsageAccess::draw:
                    return GL_STREAM_DRAW;
                    break;
                case BufferUsageAccess::read:
                    return GL_STREAM_READ;
                    break;
                case BufferUsageAccess::copy:
                    return GL_STREAM_COPY;
                    break;
            }
            break;
        case BufferUsageFrequency::statical:
            switch (access)
            {
                case BufferUsageAccess::draw:
                    return GL_STATIC_DRAW;
                    break;
                case BufferUsageAccess::read:
                    return GL_STATIC_READ;
                    break;
                case BufferUsageAccess::copy:
                    return GL_STATIC_COPY;
                    break;
            }
            break;
        case BufferUsageFrequency::dynamic:
            switch (access)
            {
                case BufferUsageAccess::draw:
                    return GL_DYNAMIC_DRAW;
                    break;
                case BufferUsageAccess::read:
                    return GL_DYNAMIC_READ;
                    break;
                case BufferUsageAccess::copy:
                    return GL_DYNAMIC_COPY;
                    break;
            }
            break;
    }

    return GL_STATIC_DRAW;
}

template <>
GLenum buffer_type<unsigned char>()
{
    return GL_UNSIGNED_BYTE;
}

template <>
GLenum buffer_type<int>()
{
    return GL_INT;
}

template <>
GLenum buffer_type<glm::ivec2>()
{
    return GL_INT;
}

template <>
GLenum buffer_type<glm::ivec3>()
{
    return GL_INT;
}

template <>
GLenum buffer_type<glm::ivec4>()
{
    return GL_INT;
}

template <>
GLenum buffer_type<unsigned int>()
{
    return GL_UNSIGNED_INT;
}

template <>
GLenum buffer_type<float>()
{
    return GL_FLOAT;
}

template <>
GLenum buffer_type<glm::vec2>()
{
    return GL_FLOAT;
}

template <>
GLenum buffer_type<glm::vec3>()
{
    return GL_FLOAT;
}

template <>
GLenum buffer_type<glm::vec4>()
{
    return GL_FLOAT;
}

template <>
unsigned int buffer_dimension<unsigned char>()
{
    return 1;
}

template <>
unsigned int buffer_dimension<int>()
{
    return 1;
}

template <>
unsigned int buffer_dimension<glm::ivec2>()
{
    return 2;
}

template <>
unsigned int buffer_dimension<glm::ivec3>()
{
    return 3;
}

template <>
unsigned int buffer_dimension<glm::ivec4>()
{
    return 4;
}

template <>
unsigned int buffer_dimension<unsigned int>()
{
    return 1;
}

template <>
unsigned int buffer_dimension<float>()
{
    return 1;
}

template <>
unsigned int buffer_dimension<glm::vec2>()
{
    return 2;
}

template <>
unsigned int buffer_dimension<glm::vec3>()
{
    return 3;
}

template <>
unsigned int buffer_dimension<glm::vec4>()
{
    return 4;
}
