#ifndef OPENGL_SHADER_UNIFORM_SUPPORT_HPP
#define OPENGL_SHADER_UNIFORM_SUPPORT_HPP

#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct AttributeInformation
{
	unsigned int    index;
	unsigned int	size;
	GLenum          type;
};

struct UniformInformation
{
	unsigned int 	location;
	unsigned int 	size;
	GLenum          type;
};

std::map<std::string, AttributeInformation>     get_attribute_information(const unsigned int id);
unsigned int									get_attribute_location(const unsigned int id, const std::string &name);

std::map<std::string, UniformInformation>		get_uniform_information(const unsigned int id);
unsigned int									get_uniform_location(const unsigned int id, const std::string &name);

template <class T> T							get_uniform(const unsigned int id, const unsigned int location, const unsigned int count);
template <class T> void							set_uniform(const unsigned int location, const T &value);

#endif
