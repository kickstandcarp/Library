#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <GL/glew.h>
#include "opengl/vertex_array.hpp"
#include "opengl/buffer_support.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/uniform_support.hpp"

enum class ShaderType : GLenum { vertex = GL_VERTEX_SHADER, geometry = GL_GEOMETRY_SHADER, fragment = GL_FRAGMENT_SHADER };

class Shader
{
    public:
		Shader(const std::vector<std::pair<std::string, ShaderType> > &shaders, const std::vector<std::string> &transform_feedback_varying_names);
		~Shader();

        unsigned int                                            get_id() const;
		std::vector<std::string>                                get_attribute_names() const;
		std::vector<std::string>                                get_transform_feedback_varying_names() const;
		std::vector<std::string>                                get_uniform_names() const;
		std::vector<std::string>                                get_uniform_buffer_names() const;
		GLenum                                                  get_uniform_type(const std::string &name) const;
		unsigned int                                            get_uniform_size(const std::string &name) const;
        template <class T> T                                    get_uniform(const std::string &name, const unsigned int offset) const;
        template <class T> std::vector<T>                       get_uniform(const std::string &name, const unsigned int offset, const unsigned int size) const;

		void                                                    set_attribute(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const;
		void                                                    set_transform_feedback_varying(const std::string &name, VertexArray &vertex_array, const std::string &buffer_name) const;
		template <class T> void                                 set_uniform(const std::string &name, const T &value, const unsigned int offset);
		template <class T> void                                 set_uniform(const std::string &name, const std::vector<T> &values, const unsigned int offset);

		void                                                    initialize_uniform_buffer(const std::string &name, const unsigned int binding, const BufferUsageFrequency frequency=BufferUsageFrequency::statical, const BufferUsageAccess access=BufferUsageAccess::draw);

	private:
		std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_uniform_information(const std::string &name) const;

        unsigned int                                            id;
		std::vector<unsigned int>                               shader_ids;

		std::map<std::string, AttributeInformation>             attribute_information;
		std::map<std::string, AttributeInformation>             transform_feedback_varying_information;
		std::map<std::string, UniformInformation>               uniform_information;
		std::map<std::string, UniformBufferInformation>         uniform_buffer_information;
};



template <class T>
T Shader::get_uniform(const std::string &name, const unsigned int offset) const
{
	T value;

	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
        value = ::get_uniform<T>(this->id, information->second.location + offset, information->second.size);
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);
        value = std::get<0>(buffer_information)->get<T>(std::get<1>(buffer_information).location + offset*sizeof(T), true);
    }

    return value;
}

template <class T>
std::vector<T> Shader::get_uniform(const std::string &name, const unsigned int offset, const unsigned int size) const
{
	std::vector<T> values;

	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
    {
        if (offset + size > information->second.size)
            throw std::length_error("uniform length error");
        values = ::get_uniform<std::vector<T> >(this->id, information->second.location + offset, size);
    }
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);

        std::vector<unsigned char> data(size*std::get<1>(buffer_information).stride);
        data = std::get<0>(buffer_information)->get<unsigned char>(std::get<1>(buffer_information).location + offset*sizeof(T), size*std::get<1>(buffer_information).stride, true);

        values.resize(size);
        for (unsigned int i = 0; i < size; i++)
            std::memcpy(static_cast<void*>(&values.data()[i]), static_cast<const void*>(&data.data()[i*std::get<1>(buffer_information).stride]), sizeof(T));
    }

    return values;
}

template <class T>
void Shader::set_uniform(const std::string &name, const T &value, const unsigned int offset)
{
	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
        ::set_uniform<T>(information->second.location + offset, value);
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);
        std::get<0>(buffer_information)->set<T>(value, std::get<1>(buffer_information).location + offset*sizeof(T), true);
    }
}

template <class T> void Shader::set_uniform(const std::string &name, const std::vector<T> &values, const unsigned int offset)
{
	auto information = this->uniform_information.find(name);
	if (information != this->uniform_information.end())
    {
        if (offset + values.size() > information->second.size)
            throw std::length_error("uniform length error");

        ::set_uniform<std::vector<T> >(information->second.location + offset, values);
    }
    else
    {
        std::tuple<std::shared_ptr<Buffer>, UniformInformation> buffer_information = this->buffer_uniform_information(name);

        std::vector<unsigned char> data(values.size()*std::get<1>(buffer_information).stride);
        for (unsigned int i = 0; i < values.size(); i++)
            std::memcpy(static_cast<void*>(&data.data()[i*std::get<1>(buffer_information).stride]), static_cast<const void*>(&values.data()[i]), sizeof(T));

        std::get<0>(buffer_information)->set<unsigned char>(data, std::get<1>(buffer_information).location + offset*sizeof(T), true);
    }
}

#endif
