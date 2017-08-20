#ifndef OPENGL_BUFFER_SUPPORT_HPP
#define OPENGL_BUFFER_SUPPORT_HPP

#include <GL/glew.h>

struct BufferInformation
{
	unsigned int    id;
	unsigned int    size;
	unsigned int    dimension;
	GLenum          type;

	int             attribute_index;
};

template <class T> GLenum       buffer_type();
template <class T> unsigned int buffer_dimension();

#endif
