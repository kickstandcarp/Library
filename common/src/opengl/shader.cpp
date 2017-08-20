#include "opengl/shader.hpp"
#include "opengl/shader_support.hpp"



Shader::Shader(const std::vector<std::pair<std::string, ShaderType> > &shaders)
{
	for (auto const &shader : shaders)
	{
		unsigned int shader_id = compile_shader(shader.first, static_cast<GLenum>(shader.second));
		this->shader_ids.push_back(shader_id);
	}
    this->id = link_shader(this->shader_ids);

	this->attribute_information = get_attribute_information(this->id);
	this->uniform_information = get_uniform_information(this->id);
}

Shader::~Shader()
{
	for (auto const &shader_id : this->shader_ids)
		glDeleteShader(shader_id);

    glDeleteProgram(this->id);
}

unsigned int Shader::get_id() const
{
	return this->id;
}

std::vector<std::string> Shader::get_attribute_names() const
{
	std::vector<std::string> names;
	for (auto const &information : this->attribute_information)
		names.push_back(information.first);
	return names;
}

std::vector<std::string> Shader::get_uniform_names() const
{
    std::vector<std::string> names;
    for (auto const &information : this->uniform_information)
        names.push_back(information.first);
    return names;
}

GLenum Shader::get_uniform_type(const std::string &name) const
{
    return this->uniform_information.at(name).type;
}

unsigned int Shader::get_uniform_size(const std::string &name) const
{
	return this->uniform_information.at(name).size;
}

void Shader::set_attribute(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const
{
    int attribute_index = this->attribute_information.at(name).index;
	if (vertex_array.get_buffer_attribute_index(buffer_name) != attribute_index)
		vertex_array.set_buffer_attribute_index(buffer_name, attribute_index);
}
