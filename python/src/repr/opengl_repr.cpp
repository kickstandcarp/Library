#include <sstream>

#include "repr/opengl_repr.hpp"
#include "repr/repr.hpp"



std::string window_repr(const Window &window)
{
	std::stringstream stream;
	stream << "Window(";
	stream << "size=(" << to_python_string<unsigned int>(window.get_size()[0]) << ", " << to_python_string<unsigned int>(window.get_size()[1]) << "))";
	return stream.str();
}

std::string shader_repr(const Shader &shader)
{
	std::stringstream stream;
	stream << "Shader()";
	return stream.str();
}

std::string vertex_array_repr(const VertexArray &vertex_array)
{
	std::stringstream stream;
	stream << "VertexArray()";
	return stream.str();
}

std::string	texture_repr(const Texture &texture)
{
	std::stringstream stream;
	stream << "Texture(";
	stream << "size=(" << to_python_string<unsigned int>(texture.get_size()[0]) << ", " << to_python_string<unsigned int>(texture.get_size()[1]) << "))";
	return stream.str();
}

std::string	frame_buffer_repr(const FrameBuffer &frame_buffer)
{
	std::stringstream stream;
	stream << "FrameBuffer(";
	stream << "size=(" << to_python_string<unsigned int>(frame_buffer.get_size()[0]) << ", " << to_python_string<unsigned int>(frame_buffer.get_size()[1]) << "))";
	return stream.str();
}
