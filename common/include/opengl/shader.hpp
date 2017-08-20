#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include <stdexcept>
#include <utility>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include "opengl/vertex_array.hpp"
#include "opengl/uniform_support.hpp"
#include "opengl/buffer_support.hpp"

enum class ShaderType : GLenum { vertex = GL_VERTEX_SHADER, geometry = GL_GEOMETRY_SHADER, fragment = GL_FRAGMENT_SHADER };

class Shader
{
    public:
		Shader(const std::vector<std::pair<std::string, ShaderType> > &shaders);
		~Shader();

        unsigned int                                get_id() const;
		std::vector<std::string>					get_attribute_names() const;
		std::vector<std::string>					get_uniform_names() const;
		GLenum										get_uniform_type(const std::string &name) const;
		unsigned int								get_uniform_size(const std::string &name) const;
        template<class T> T							get_uniform(const std::string &name) const;

		template<class T> void						set_uniform(const std::string &name, const T &value);
		void										set_attribute(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const;

    private:
        unsigned int								id;
		std::vector<unsigned int>					shader_ids;

		std::map<std::string, AttributeInformation>	attribute_information;
		std::map<std::string, UniformInformation>	uniform_information;
};



template <class T>
T Shader::get_uniform(const std::string &name) const
{
    T value = ::get_uniform<T>(this->id, this->uniform_information.at(name).location, this->uniform_information.at(name).size);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::stringstream what;
        what << "uniform get error: " << name << std::endl << glewGetErrorString(error);
        throw std::runtime_error(what.str());
	}

    return value;
}

template <class T>
void Shader::set_uniform(const std::string &name, const T &value)
{
    ::set_uniform<T>(this->uniform_information.at(name).location, value);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::stringstream what;
        what << "uniform set error: " << name << std::endl << glewGetErrorString(error);
        throw std::runtime_error(what.str());
	}
}

#endif
