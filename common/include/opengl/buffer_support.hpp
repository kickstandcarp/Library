#ifndef OPENGL_BUFFER_SUPPORT_HPP
#define OPENGL_BUFFER_SUPPORT_HPP

#include <GL/glew.h>

enum class BufferUsageFrequency : int { stream, statical, dynamic }; 
enum class BufferUsageAccess : int { draw, read, copy };

// unsigned int                        type_size(const GLenum type);

GLenum								buffer_usage(const BufferUsageFrequency frequency=BufferUsageFrequency::statical, const BufferUsageAccess access=BufferUsageAccess::draw);
template <class T> GLenum			buffer_type();
template <class T> unsigned int		buffer_dimension();

#endif
