#ifndef OPENGL_VERTEX_ARRAY_HPP
#define OPENGL_VERTEX_ARRAY_HPP

#include <stdexcept>
#include <utility>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include "opengl/buffer_support.hpp"

enum class DrawMode : GLenum { points = GL_POINTS, line_strip = GL_LINE_STRIP, line_loop = GL_LINE_LOOP, lines = GL_LINES, triangle_strip = GL_TRIANGLE_STRIP, triangle_fan = GL_TRIANGLE_FAN, triangles = GL_TRIANGLES }; 

class VertexArray
{
    public:
        VertexArray(const DrawMode draw_mode);
        ~VertexArray();

		unsigned int								get_id() const;
		std::vector<std::string>					get_buffer_names() const;
		unsigned int								get_buffer_size(const std::string &name) const;
		unsigned int								get_element_buffer_size() const;
		GLenum                                      get_buffer_type(const std::string &name) const;
		unsigned int								get_buffer_dimension(const std::string &name) const;
		int                                         get_buffer_attribute_index(const std::string &name) const;
		template <class T> std::vector<T>			get_buffer(const std::string &name, const unsigned int offset, const unsigned int size) const;
		std::vector<unsigned int>					get_element_buffer(const unsigned int offset, const unsigned int size) const;

		void										set_buffer_attribute_index(const std::string &name, const int index);
		template <class T> void						set_buffer(const std::string &name, const unsigned int offset, const std::vector<T> &data);
		void										set_element_buffer(const unsigned int offset, const std::vector<unsigned int> &data);

		template <class T> void						add_buffer(const std::string &name, const std::vector<T> &data);
		void										add_element_buffer(const std::vector<unsigned int> &data);

		void										remove_buffer(const std::string &name);
		void										remove_element_buffer();

		void										draw(const unsigned int count=1) const;

		DrawMode									draw_mode;

    private:
        unsigned int								id;
	
		BufferInformation							element_buffer_information;
		std::map<std::string, BufferInformation>	buffer_information;
};



template<class T>
std::vector<T> VertexArray::get_buffer(const std::string &name, const unsigned int offset, const unsigned int size) const
{
	if (offset + size > this->buffer_information.at(name).size)
	{
		std::stringstream what;
		what << "buffer length error: " << name;
		throw std::length_error(what.str());
	}

	std::vector<T> data(size);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_information.at(name).id);
	glGetBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(T), size*sizeof(T), data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return data;
}

template<class T>
void VertexArray::set_buffer(const std::string &name, const unsigned int offset, const std::vector<T> &data)
{
	if (offset + data.size() > this->buffer_information.at(name).size)
	{
		std::stringstream what;
		what << "buffer length error: " << name;
		throw std::length_error(what.str());
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_information.at(name).id);
	glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(T), data.size()*sizeof(T), data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<class T>
void VertexArray::add_buffer(const std::string &name, const std::vector<T> &data)
{
	unsigned int buffer_id;
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->buffer_information.insert(std::make_pair(name, BufferInformation{buffer_id, static_cast<unsigned int>(data.size()), buffer_dimension<T>(), buffer_type<T>(), -1}));
}

#endif
