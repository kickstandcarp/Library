#include <stdexcept>
#include <sstream>

#include "opengl/shader.hpp"



Shader::Shader(const std::vector<std::pair<std::string, ShaderType> > &shaders, const std::vector<std::string> &transform_feedback_varying_names)
{
	for (auto const &shader : shaders)
	{
		unsigned int shader_id = compile_shader(shader.first, static_cast<GLenum>(shader.second));
		this->shader_ids.push_back(shader_id);
	}
    this->id = attach_program(this->shader_ids);
	
	if (transform_feedback_varying_names.size() != 0)
		assign_transform_feedback_varyings(this->id, transform_feedback_varying_names);

	link_program(this->id);

	this->attribute_information = ::attribute_information(this->id);
	this->transform_feedback_varying_information = ::transform_feedback_varying_information(this->id);
	this->uniform_information = ::uniform_information(this->id);
	this->uniform_buffer_information = ::uniform_buffer_information(this->id);
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

std::vector<std::string> Shader::get_transform_feedback_varying_names() const
{
	std::vector<std::string> names;
	for (auto const &information : this->transform_feedback_varying_information)
		names.push_back(information.first);
	return names;
}

std::vector<std::string> Shader::get_uniform_names() const
{
    std::vector<std::string> names;
    for (auto const &information : this->uniform_information)
        names.push_back(information.first);
    for (auto const &buffer_information : this->uniform_buffer_information)
    {
        for (auto const &information : buffer_information.second.uniform_information)
            names.push_back(information.first);
    }

    return names;
}

std::vector<std::string> Shader::get_uniform_buffer_names() const
{
	std::vector<std::string> names;
	for (auto const &information : this->uniform_buffer_information)
		names.push_back(information.first);
	return names;
}

GLenum Shader::get_uniform_type(const std::string &name) const
{
	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
        return information->second.type;
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);
        return std::get<1>(buffer_information).type;
    }
}

unsigned int Shader::get_uniform_size(const std::string &name) const
{
	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
        return information->second.size;
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);
        return std::get<1>(buffer_information).size;
    }
}

void Shader::set_attribute(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const
{
    int attribute_index = this->attribute_information.at(name).index;
	if (vertex_array.get_buffer_attribute_index(buffer_name) != attribute_index)
		vertex_array.set_buffer_attribute_index(buffer_name, attribute_index);
}

void Shader::set_transform_feedback_varying(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const
{
	int transform_feedback_varying_index = this->transform_feedback_varying_information.at(name).index;
	if (vertex_array.get_buffer_transform_feedback_varying_index(buffer_name) != transform_feedback_varying_index)
		vertex_array.set_buffer_transform_feedback_varying_index(buffer_name, transform_feedback_varying_index);
}

void Shader::initialize_uniform_buffer(const std::string &name, const unsigned int binding, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    this->uniform_buffer_information.at(name).binding = binding;

    std::vector<unsigned char> data(this->uniform_buffer_information.at(name).size, 0);
    this->uniform_buffer_information.at(name).buffer = std::make_shared<Buffer>();
    this->uniform_buffer_information.at(name).buffer->initialize(data, frequency, access);

    glBindBufferBase(GL_UNIFORM_BUFFER, this->uniform_buffer_information.at(name).binding, this->uniform_buffer_information.at(name).buffer->get_id());
    glUniformBlockBinding(this->id, this->uniform_buffer_information.at(name).index, this->uniform_buffer_information.at(name).binding);
}

std::tuple<std::shared_ptr<Buffer>, UniformInformation> Shader::buffer_uniform_information(const std::string &name) const
{
    for (auto const &buffer_information : this->uniform_buffer_information)
    {
        auto information = buffer_information.second.uniform_information.find(name);
        if (information != buffer_information.second.uniform_information.end())
        {
            if (buffer_information.second.buffer)
                return std::make_tuple(buffer_information.second.buffer, information->second);
            else
                throw std::runtime_error("uniform buffer uninitialized");
        }
    }

	std::stringstream what;
	what << "invalid uniform name: " << name << std::endl;
	throw std::runtime_error(what.str());
}
