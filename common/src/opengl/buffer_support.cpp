#include <glm/glm.hpp>

#include "opengl/buffer_support.hpp"



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
