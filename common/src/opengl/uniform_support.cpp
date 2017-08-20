#include <stdexcept>
#include <utility>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "opengl/uniform_support.hpp"



std::map<std::string, AttributeInformation> get_attribute_information(const unsigned int id)
{
	std::map<std::string, AttributeInformation> information;

	int num_attributes;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &num_attributes);

	int size;
	GLenum type;
	char name_c_str[1024];
	for (int i = 0; i < num_attributes; i++)
	{
		glGetActiveAttrib(id, i, 1024, NULL, &size, &type, name_c_str);
		std::string name = std::string(name_c_str);
		information.insert(std::make_pair(name, AttributeInformation{get_attribute_location(id, name), static_cast<unsigned int>(size), type}));
	}

	return information;
}

unsigned int get_attribute_location(const unsigned int id, const std::string &name)
{
	int location = glGetAttribLocation(id, name.c_str());

	if (location == -1)
	{
		std::stringstream what;
		what << "attribute location error: " << name;
		throw std::runtime_error(what.str());
	}

	return static_cast<unsigned int>(location);
}

std::map<std::string, UniformInformation> get_uniform_information(const unsigned int id)
{
    std::map<std::string, UniformInformation> information;

    int num_uniforms;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &num_uniforms);

	int size;
	GLenum type;
	char name_c_str[1024];
    for (int i = 0; i < num_uniforms; i++)
    {
        glGetActiveUniform(id, i, 1024, NULL, &size, &type, name_c_str);
		
		std::string name(name_c_str);
		if (size > 1)
			name = name.substr(0, name.find_first_of("["));

		information.insert(std::make_pair(name, UniformInformation{get_uniform_location(id, name), static_cast<unsigned int>(size), type}));
    }

    return information;
}

unsigned int get_uniform_location(const unsigned int id, const std::string &name)
{
	int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
	{
        std::stringstream what;
        what << "uniform location error: " << name;
        throw std::runtime_error(what.str());
	}

	return static_cast<unsigned int>(location);
}

template <>
int get_uniform<int>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    int value;
	glGetUniformiv(id, location, &value);
    return value;
}

template <>
glm::ivec2 get_uniform<glm::ivec2>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::ivec2 value;
	glGetUniformiv(id, location, glm::value_ptr(value));
    return value;
}

template <>
glm::ivec3 get_uniform<glm::ivec3>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::ivec3 value;
	glGetUniformiv(id, location, glm::value_ptr(value));
    return value;
}

template <>
glm::ivec4 get_uniform<glm::ivec4>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::ivec4 value;
	glGetUniformiv(id, location, glm::value_ptr(value));
    return value;
}

template <>
float get_uniform<float>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    float value;
	glGetUniformfv(id, location, &value);
    return value;
}

template <>
glm::vec2 get_uniform<glm::vec2>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::vec2 value;
	glGetUniformfv(id, location, glm::value_ptr(value));
    return value;
}

template <>
glm::vec3 get_uniform<glm::vec3>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::vec3 value;
	glGetUniformfv(id, location, glm::value_ptr(value));
    return value;
}

template <>
glm::vec4 get_uniform<glm::vec4>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::vec4 value;
	glGetUniformfv(id, location, glm::value_ptr(value));
    return value;
}

template <>
glm::mat4x4 get_uniform<glm::mat4x4>(const unsigned int id, const unsigned int location, const unsigned int count)
{
    glm::mat4x4 value;
	glGetUniformfv(id, location, glm::value_ptr(value));
    return value;
}

template <>
std::vector<int> get_uniform<std::vector<int> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<int> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformiv(id, location+i, &value[i]);
	return value;
}

template <>
std::vector<glm::ivec2> get_uniform<std::vector<glm::ivec2> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::ivec2> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformiv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<glm::ivec3> get_uniform<std::vector<glm::ivec3> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::ivec3> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformiv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<glm::ivec4> get_uniform<std::vector<glm::ivec4> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::ivec4> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformiv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<float> get_uniform<std::vector<float> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<float> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformfv(id, location+i, &value[i]);
	return value;
}

template <>
std::vector<glm::vec2> get_uniform<std::vector<glm::vec2> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::vec2> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformfv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<glm::vec3> get_uniform<std::vector<glm::vec3> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::vec3> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformfv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<glm::vec4> get_uniform<std::vector<glm::vec4> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::vec4> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformfv(id, location+i, &value[i][0]);
	return value;
}

template <>
std::vector<glm::mat4x4> get_uniform<std::vector<glm::mat4x4> >(const unsigned int id, const unsigned int location, const unsigned int count)
{
	std::vector<glm::mat4x4> value(count);
	for (unsigned int i = 0; i < value.size(); i++)
		glGetUniformfv(id, location+i, &value[i][0][0]);
	return value;
}

template <>
void set_uniform<int>(const unsigned int location, const int &value)
{
	glUniform1i(location, value);
}

template <>
void set_uniform<glm::ivec2>(const unsigned int location, const glm::ivec2 &value)
{
	glUniform2iv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<glm::ivec3>(const unsigned int location, const glm::ivec3 &value)
{
	glUniform3iv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<glm::ivec4>(const unsigned int location, const glm::ivec4 &value)
{
	glUniform4iv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<float>(const unsigned int location, const float &value)
{
	glUniform1fv(location, 1, &value);
}

template <>
void set_uniform<glm::vec2>(const unsigned int location, const glm::vec2 &value)
{
	glUniform2fv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<glm::vec3>(const unsigned int location, const glm::vec3 &value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<glm::vec4>(const unsigned int location, const glm::vec4 &value)
{
	glUniform4fv(location, 1, glm::value_ptr(value));
}

template <>
void set_uniform<glm::mat4x4>(const unsigned int location, const glm::mat4x4 &value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void set_uniform<std::vector<int> >(const unsigned int location, const std::vector<int> &value)
{
	glUniform1iv(location, value.size(), &value[0]);
}

template <>
void set_uniform<std::vector<glm::ivec2> >(const unsigned int location, const std::vector<glm::ivec2> &value)
{
	glUniform2iv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<glm::ivec3> >(const unsigned int location, const std::vector<glm::ivec3> &value)
{
	glUniform3iv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<glm::ivec4> >(const unsigned int location, const std::vector<glm::ivec4> &value)
{
	glUniform4iv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<float> >(const unsigned int location, const std::vector<float> &value)
{
	glUniform1fv(location, value.size(), &value[0]);
}

template <>
void set_uniform<std::vector<glm::vec2> >(const unsigned int location, const std::vector<glm::vec2> &value)
{
	glUniform2fv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<glm::vec3> >(const unsigned int location, const std::vector<glm::vec3> &value)
{
	glUniform3fv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<glm::vec4> >(const unsigned int location, const std::vector<glm::vec4> &value)
{
	glUniform4fv(location, value.size(), &value[0][0]);
}

template <>
void set_uniform<std::vector<glm::mat4x4> >(const unsigned int location, const std::vector<glm::mat4x4> &value)
{
	glUniformMatrix4fv(location, value.size(), GL_FALSE, &value[0][0][0]);
}
