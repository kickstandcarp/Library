#include "opengl/vertex_array.hpp"



VertexArray::VertexArray(const DrawMode draw_mode)
:	draw_mode(draw_mode),
	element_buffer_information({0, 0, GL_UNSIGNED_INT, 1, -1})
{
    glGenVertexArrays(1, &this->id);
}

VertexArray::~VertexArray()
{
	for (auto const &information : this->buffer_information)
		glDeleteBuffers(1, &information.second.id);

	if (this->element_buffer_information.id != 0)
		glDeleteBuffers(1, &this->element_buffer_information.id);

    glDeleteVertexArrays(1, &this->id);
}

unsigned int VertexArray::get_id() const
{
	return this->id;
}

std::vector<std::string> VertexArray::get_buffer_names() const
{
	std::vector<std::string> names;
	for (auto const &information : this->buffer_information)
		names.push_back(information.first);
	return names;
}

unsigned int VertexArray::get_buffer_size(const std::string &name) const
{
	return this->buffer_information.at(name).size;
}

unsigned int VertexArray::get_element_buffer_size() const
{
	return this->element_buffer_information.size;
}

GLenum VertexArray::get_buffer_type(const std::string &name) const
{
	return this->buffer_information.at(name).type;
}

unsigned int VertexArray::get_buffer_dimension(const std::string &name) const
{
	return this->buffer_information.at(name).dimension;
}

int VertexArray::get_buffer_attribute_index(const std::string &name) const
{
	return this->buffer_information.at(name).attribute_index;
}

std::vector<unsigned int> VertexArray::get_element_buffer(const unsigned int offset, const unsigned int size) const
{
	if (this->element_buffer_information.id == 0 || offset + size > this->element_buffer_information.size)
		throw std::length_error("element buffer length error");

	std::vector<unsigned int> data(size, 2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer_information.id);
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset*sizeof(unsigned int), size*sizeof(unsigned int), data.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return data;
}

void VertexArray::set_buffer_attribute_index(const std::string &name, const int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_information.at(name).id);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, this->buffer_information.at(name).dimension, this->buffer_information.at(name).type, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (auto &information : this->buffer_information)
	{
		if (information.first != name && information.second.attribute_index == index)
			information.second.attribute_index = -1;
	}
	this->buffer_information.at(name).attribute_index = index;
}

void VertexArray::set_element_buffer(const unsigned int offset, const std::vector<unsigned int> &data)
{
	if (this->element_buffer_information.id == 0 || offset + data.size() > this->element_buffer_information.size)
		throw std::length_error("element buffer length error");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer_information.id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset*sizeof(unsigned int), data.size()*sizeof(unsigned int), data.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::add_element_buffer(const std::vector<unsigned int> &data)
{
	unsigned int buffer_id;
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	this->element_buffer_information = BufferInformation{buffer_id, static_cast<unsigned int>(data.size()), 1, GL_UNSIGNED_INT, -1};
}

void VertexArray::remove_buffer(const std::string &name)
{
	glDeleteBuffers(1, &this->buffer_information.at(name).id);
	this->buffer_information.erase(name);
}

void VertexArray::remove_element_buffer()
{
	glDeleteBuffers(1, &this->element_buffer_information.id);
	this->element_buffer_information = BufferInformation{0, 0, 1, GL_UNSIGNED_INT, -1};
}

void VertexArray::draw(const unsigned int count) const
{
	if (count == 1)
    {
        if (this->element_buffer_information.id != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer_information.id);
            glDrawElements(static_cast<GLenum>(this->draw_mode), this->element_buffer_information.size, GL_UNSIGNED_INT, (void*)0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
            glDrawArrays(static_cast<GLenum>(this->draw_mode), 0, 0); // this->num_vertices);
    }
    else
    {
        if (this->element_buffer_information.id != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer_information.id);
            glDrawElementsInstanced(static_cast<GLenum>(this->draw_mode), this->element_buffer_information.size, GL_UNSIGNED_INT, (void*)0, count);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
            glDrawArraysInstanced(static_cast<GLenum>(this->draw_mode), 0, 0, 0); // this->num_vertices, count);
    }
}
