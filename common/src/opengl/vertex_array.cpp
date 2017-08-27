#include <stdexcept>

#include "opengl/vertex_array.hpp"



VertexArray::VertexArray(const DrawMode draw_mode)
:	draw_mode(draw_mode),
	num_vertices(0)
{
    glGenVertexArrays(1, &this->id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->id);
}

unsigned int VertexArray::get_id() const
{
	return this->id;
}

unsigned int VertexArray::get_num_vertices() const
{
	return this->element_buffer ? element_buffer->get_size() : this->num_vertices;
}

std::vector<std::string> VertexArray::get_buffer_names() const
{
	std::vector<std::string> names;
	for (auto const &buffer : this->buffers)
		names.push_back(buffer.first);
	return names;
}

unsigned int VertexArray::get_buffer_size(const std::string &name) const
{
	return this->buffers.at(name).buffer->get_size();
}

unsigned int VertexArray::get_element_buffer_size() const
{
	return this->element_buffer->get_size();
}

GLenum VertexArray::get_buffer_type(const std::string &name) const
{
	return this->buffers.at(name).buffer->get_type();
}

unsigned int VertexArray::get_buffer_dimension(const std::string &name) const
{
	return this->buffers.at(name).buffer->get_dimension();
}

int VertexArray::get_buffer_attribute_index(const std::string &name) const
{
	return this->buffers.at(name).attribute_index;
}

int VertexArray::get_buffer_transform_feedback_varying_index(const std::string &name) const
{
	return this->buffers.at(name).transform_feedback_varying_index;
}

unsigned int VertexArray::get_element_buffer(const unsigned int offset) const
{
    return this->element_buffer->get<unsigned int>(offset, false);
}

std::vector<unsigned int> VertexArray::get_element_buffer(const unsigned int offset, const unsigned int size) const
{
	return this->element_buffer->get<unsigned int>(offset, size, false);
}

void VertexArray::set_buffer_attribute_index(const std::string &name, const int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers.at(name).buffer->get_id());
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, this->buffers.at(name).buffer->get_dimension(), this->buffers.at(name).buffer->get_type(), GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (auto &buffer : this->buffers)
	{
		if (buffer.first != name && this->buffers.at(name).attribute_index == index)
			this->buffers.at(name).attribute_index = -1;
	}
	this->buffers.at(name).attribute_index = index;
}

void VertexArray::set_buffer_transform_feedback_varying_index(const std::string &name, const int index)
{
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, this->buffers.at(name).buffer->get_id());

	for (auto &buffer : this->buffers)
	{
		if (buffer.first != name && this->buffers.at(name).transform_feedback_varying_index == index)
			this->buffers.at(name).transform_feedback_varying_index = -1;
	}
	this->buffers.at(name).transform_feedback_varying_index = index;
}

void VertexArray::set_element_buffer(const unsigned int data, const unsigned int offset)
{
    this->element_buffer->set(data, offset, false);
}

void VertexArray::set_element_buffer(const std::vector<unsigned int> &data, const unsigned int offset)
{
	this->element_buffer->set(data, offset, false);
}

void VertexArray::add_element_buffer(const std::vector<unsigned int> &data, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
	this->element_buffer = std::make_shared<Buffer>();
	this->element_buffer->initialize(data, frequency, access);
}

void VertexArray::remove_buffer(const std::string &name)
{
	this->buffers.erase(name);

	if (this->buffers.size() == 0)
		this->num_vertices = 0;
	else
	{
		for (auto const &buffer : this->buffers)
			this->num_vertices = std::min(this->num_vertices, buffer.second.buffer->get_size());
	}
}

void VertexArray::remove_element_buffer()
{
	this->element_buffer.reset();
}

void VertexArray::draw(const unsigned int count, const bool transform_feedback) const
{
	if (transform_feedback)
		glBeginTransformFeedback(this->transform_feedback_primitive_mode());

	if (count == 1)
    {
        if (this->element_buffer)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer->get_id());
            glDrawElements(static_cast<GLenum>(this->draw_mode), this->element_buffer->get_size(), this->element_buffer->get_type(), (void*)0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
			glDrawArrays(static_cast<GLenum>(this->draw_mode), 0, this->num_vertices);
    }
    else
    {
        if (this->element_buffer)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_buffer->get_id());
            glDrawElementsInstanced(static_cast<GLenum>(this->draw_mode), this->element_buffer->get_size(), this->element_buffer->get_type(), (void*)0, count);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
		   glDrawArraysInstanced(static_cast<GLenum>(this->draw_mode), 0, this->num_vertices, count);
    }

	if (transform_feedback)
		glEndTransformFeedback();
}

GLenum VertexArray::transform_feedback_primitive_mode() const
{
	switch (this->draw_mode)
	{
		case DrawMode::points:
			return GL_POINTS;
			break;
		case DrawMode::line_strip:
			return GL_LINES;
			break;
		case DrawMode::line_loop:
			return GL_LINES;
			break;
		case DrawMode::lines:
			return GL_LINES;
			break;
		case DrawMode::triangle_strip:
			return GL_TRIANGLES;
			break;
		case DrawMode::triangle_fan:
			return GL_TRIANGLES;
			break;
		case DrawMode::triangles:
			return GL_TRIANGLES;
			break;
		default:
			throw std::runtime_error("unknown draw mode");
	}
}
