#ifndef OPENGL_SHADER_UNIFORM_SUPPORT_HPP
#define OPENGL_SHADER_UNIFORM_SUPPORT_HPP

#include <memory>
#include <string>
#include <map>
#include <GL/glew.h>
#include "opengl/buffer.hpp"

struct UniformInformation
{
	unsigned int                                location;
	unsigned int                                size;
    int                                         stride;

	GLenum                                      type;
};

struct UniformBufferInformation
{
	unsigned int                                index;
	unsigned int                                size;

    unsigned int                                binding;
    std::shared_ptr<Buffer>                     buffer;

	std::map<std::string, UniformInformation>   uniform_information;
};

template <class T> T							get_uniform(const unsigned int id, const unsigned int location, const unsigned int count);
template <class T> void							set_uniform(const unsigned int location, const T &value);

std::map<std::string, UniformInformation>		uniform_information(const unsigned int id);
std::map<std::string, UniformBufferInformation> uniform_buffer_information(const unsigned int id);

#endif
