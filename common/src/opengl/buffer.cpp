#include "opengl/buffer.hpp"



Buffer::Buffer()
:	size(0),
	dimension(0),
	type(GL_NONE)
{
	glGenBuffers(1, &this->id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &this->id);
}

unsigned int Buffer::get_id() const
{
	return this->id;
}

unsigned int Buffer::get_size() const
{
	return this->size;
}

GLenum Buffer::get_type() const
{
	return this->type;
}

unsigned int Buffer::get_dimension() const
{
	return this->dimension;
}
