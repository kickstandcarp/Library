#ifndef MODULE_OPENGL_HPP
#define MODULE_OPENGL_HPP

#include <stdexcept>
#include <utility>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"
#include "image/transform.hpp"

namespace py = pybind11;

py::object															py_get_uniform(const Shader &shader, const std::string &name);
void																py_set_uniform(Shader &shader, const std::string &name, const py::handle &value);

py::object															py_get_buffer(const VertexArray &vertex_array, const std::string &name, const unsigned int offset, const unsigned int size);
void																py_set_buffer(VertexArray &vertex_array, const std::string &name, const unsigned int offset, const py::iterable &data);
void																py_add_buffer(VertexArray &vertex_array, const std::string &name, const py::iterable &data);
template <class T> bool												py_add_buffer_type(VertexArray &vertex_array, const std::string &name, const py::iterable &data);

std::tuple<std::vector<float>, const std::array<unsigned int, 2> >	py_array_to_texture_data(const py::array_t<float> &image);
py::array_t<float>													py_texture_data_to_array(const std::vector<float> &data, const std::array<unsigned int, 3> &shape);



py::object py_get_uniform(const Shader &shader, const std::string &name)
{
    switch (shader.get_uniform_type(name))
    {
        case GL_INT:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<int> >(shader.get_uniform<std::vector<int> >(name));
			else
				return py::cast<int>(shader.get_uniform<int>(name));
            break;
        case GL_INT_VEC2:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::ivec2> >(shader.get_uniform<std::vector<glm::ivec2> >(name));
			else
				return py::cast<glm::ivec2>(shader.get_uniform<glm::ivec2>(name));
            break;
        case GL_INT_VEC3:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::ivec3> >(shader.get_uniform<std::vector<glm::ivec3> >(name));
			else
				return py::cast<glm::ivec3>(shader.get_uniform<glm::ivec3>(name));
            break;
        case GL_INT_VEC4:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::ivec4> >(shader.get_uniform<std::vector<glm::ivec4> >(name));
			else
	            return py::cast<glm::ivec4>(shader.get_uniform<glm::ivec4>(name));
            break;
        case GL_FLOAT:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<float> >(shader.get_uniform<std::vector<float> >(name));
			else
				return py::cast<float>(shader.get_uniform<float>(name));
            break;
        case GL_FLOAT_VEC2:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::vec2> >(shader.get_uniform<std::vector<glm::vec2> >(name));
			else
				return py::cast<glm::vec2>(shader.get_uniform<glm::vec2>(name));
            break;
        case GL_FLOAT_VEC3:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::vec3> >(shader.get_uniform<std::vector<glm::vec3> >(name));
			else
				return py::cast<glm::vec3>(shader.get_uniform<glm::vec3>(name));
            break;
        case GL_FLOAT_VEC4:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::vec4> >(shader.get_uniform<std::vector<glm::vec4> >(name));
			else
				return py::cast<glm::vec4>(shader.get_uniform<glm::vec4>(name));
            break;
        case GL_FLOAT_MAT4:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<glm::mat4x4> >(shader.get_uniform<std::vector<glm::mat4x4> >(name));
			else
				return py::cast<glm::mat4x4>(shader.get_uniform<glm::mat4x4>(name));
            break;
        case GL_SAMPLER_2D:
			if (shader.get_uniform_size(name) > 1)
				return py::cast<std::vector<int> >(shader.get_uniform<std::vector<int> >(name));
			else
				return py::cast<int>(shader.get_uniform<int>(name));
            break;
        default:
        {
            std::stringstream what;
            what << "unknown uniform type: '" << name << "'";
            throw std::runtime_error(what.str());
        }
    }
}

void py_set_uniform(Shader &shader, const std::string &name, const py::handle &value)
{
    switch (shader.get_uniform_type(name))
    {
        case GL_INT:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<int> >(name, value.cast<std::vector<int> >());
			else
				shader.set_uniform<int>(name, value.cast<int>());
            break;
        case GL_INT_VEC2:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::ivec2> >(name, value.cast<std::vector<glm::ivec2> >());
			else
				shader.set_uniform<glm::ivec2>(name, value.cast<glm::ivec2>());
            break;
        case GL_INT_VEC3:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::ivec3> >(name, value.cast<std::vector<glm::ivec3> >());
			else
				shader.set_uniform<glm::ivec3>(name, value.cast<glm::ivec3>());
            break;
        case GL_INT_VEC4:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::ivec4> >(name, value.cast<std::vector<glm::ivec4> >());
			else
				shader.set_uniform<glm::ivec4>(name, value.cast<glm::ivec4>());
            break;
        case GL_FLOAT:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<float> >(name, value.cast<std::vector<float> >());
			else
				shader.set_uniform<float>(name, value.cast<float>());
            break;
        case GL_FLOAT_VEC2:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::vec2> >(name, value.cast<std::vector<glm::vec2> >());
			else
				shader.set_uniform<glm::vec2>(name, value.cast<glm::vec2>());
            break;
        case GL_FLOAT_VEC3:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::vec3> >(name, value.cast<std::vector<glm::vec3> >());
			else
				shader.set_uniform<glm::vec3>(name, value.cast<glm::vec3>());
            break;
        case GL_FLOAT_VEC4:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::vec4> >(name, value.cast<std::vector<glm::vec4> >());
			else
	            shader.set_uniform<glm::vec4>(name, value.cast<glm::vec4>());
            break;
        case GL_FLOAT_MAT4:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<glm::mat4x4> >(name, value.cast<std::vector<glm::mat4x4> >());
			else
				shader.set_uniform<glm::mat4x4>(name, value.cast<glm::mat4x4>());
            break;
        case GL_SAMPLER_2D:
			if (shader.get_uniform_size(name) > 1)
				shader.set_uniform<std::vector<int> >(name, value.cast<std::vector<int> >());
			else
				shader.set_uniform<int>(name, value.cast<int>());
            break;
        default:
        {
            std::stringstream what;
            what << "unknown uniform type: '" << name << "'";
            throw std::runtime_error(what.str());
        }
    }
}

py::object py_get_buffer(const VertexArray &vertex_array, const std::string &name, const unsigned int offset, const unsigned int size)
{
	GLenum type = vertex_array.get_buffer_type(name);
	unsigned int dimension = vertex_array.get_buffer_dimension(name);
	switch (type)
	{
		case GL_INT:
			if (dimension == 1)
				return py::cast<std::vector<int> >(vertex_array.get_buffer<int>(name, offset, size));
			else if (dimension == 2)
				return py::cast<std::vector<glm::ivec2> >(vertex_array.get_buffer<glm::ivec2>(name, offset, size));
			else if (dimension == 3)
				return py::cast<std::vector<glm::ivec3> >(vertex_array.get_buffer<glm::ivec3>(name, offset, size));
			else if (dimension == 4)
				return py::cast<std::vector<glm::ivec4> >(vertex_array.get_buffer<glm::ivec4>(name, offset, size));
			else
			{
				std::stringstream what;
				what << "invalid buffer dimension: '" << name << "'";
				throw std::runtime_error(what.str());
			}
			break;
		case GL_FLOAT:
			if (dimension == 1)
				return py::cast<std::vector<float> >(vertex_array.get_buffer<float>(name, offset, size));
			else if (dimension == 2)
				return py::cast<std::vector<glm::vec2> >(vertex_array.get_buffer<glm::vec2>(name, offset, size));
			else if (dimension == 3)
				return py::cast<std::vector<glm::vec3> >(vertex_array.get_buffer<glm::vec3>(name, offset, size));
			else if (dimension == 4)
				return py::cast<std::vector<glm::vec4> >(vertex_array.get_buffer<glm::vec4>(name, offset, size));
			else
			{
				std::stringstream what;
				what << "invalid buffer dimension: '" << name << "'";
				throw std::runtime_error(what.str());
			}
			break;
		default:
		{
			std::stringstream what;
			what << "unknown buffer type: '" << name << "'";
			throw std::runtime_error(what.str());
		}
	}
}

void py_set_buffer(VertexArray &vertex_array, const std::string &name, const unsigned int offset, const py::iterable &data)
{
	std::string buffer_type;
	GLenum type = vertex_array.get_buffer_type(name);
	unsigned int dimension = vertex_array.get_buffer_dimension(name);
	try
	{
		switch (type)
		{
			case GL_INT:
				if (dimension == 1)
				{
					buffer_type = "int";
					vertex_array.set_buffer<int>(name, offset, data.cast<std::vector<int> >());
				}
				else if (dimension == 2)
				{
					buffer_type = "ivec2";
					vertex_array.set_buffer<glm::ivec2>(name, offset, data.cast<std::vector<glm::ivec2> >());
				}
				else if (dimension == 3)
				{
					buffer_type = "ivec3";
					vertex_array.set_buffer<glm::ivec3>(name, offset, data.cast<std::vector<glm::ivec3> >());
				}
				else if (dimension == 4)
				{
					buffer_type = "ivec4";
					vertex_array.set_buffer<glm::ivec4>(name, offset, data.cast<std::vector<glm::ivec4> >());
				}
				else
				{
					std::stringstream what;
					what << "invalid buffer dimension: '" << name << "'";
					throw std::runtime_error(what.str());
				}
				break;
			case GL_FLOAT:
				if (dimension == 1)
				{
					buffer_type = "float";
					vertex_array.set_buffer<float>(name, offset, data.cast<std::vector<float> >());
				}
				else if (dimension == 2)
				{
					buffer_type = "vec2";
					vertex_array.set_buffer<glm::vec2>(name, offset, data.cast<std::vector<glm::vec2> >());
				}
				else if (dimension == 3)
				{
					buffer_type = "vec3";
					vertex_array.set_buffer<glm::vec3>(name, offset, data.cast<std::vector<glm::vec3> >());
				}
				else if (dimension == 4)
				{
					buffer_type = "vec4";
					vertex_array.set_buffer<glm::vec4>(name, offset, data.cast<std::vector<glm::vec4> >());
				}
				else
				{
					std::stringstream what;
					what << "invalid buffer dimension: '" << name << "'";
					throw std::runtime_error(what.str());
				}
				break;
			default:
			{
				std::stringstream what;
				what << "unknown buffer type: '" << name << "'";
				throw std::runtime_error(what.str());
			}
		}
	}
	catch (py::cast_error)
	{
		std::stringstream what;
		what << "'" << name<< "' buffer type is: " << buffer_type;
		throw std::invalid_argument(what.str());
	}
}

void py_add_buffer(VertexArray &vertex_array, const std::string &name, const py::iterable &data)
{
	if (py_add_buffer_type<int>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::ivec2>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::ivec3>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::ivec4>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<float>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::vec2>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::vec3>(vertex_array, name, data))
		return;
	if (py_add_buffer_type<glm::vec4>(vertex_array, name, data))
		return;

	std::stringstream what;
	what << "unknown buffer data type: '" << name << "'";
	throw std::invalid_argument(what.str());
}

template <class T>
bool py_add_buffer_type(VertexArray &vertex_array, const std::string &name, const py::iterable &data)
{
	try
	{
		vertex_array.add_buffer<T>(name, data.cast<std::vector<T> >());
		return true;
	}
	catch(...)
	{
		return false;
	}
}

std::tuple<std::vector<float>, const std::array<unsigned int, 2> > py_array_to_texture_data(const py::array_t<float> &image)
{
	py::buffer_info info = const_cast<py::array_t<float>&>(image).request();

	std::vector<std::size_t> shape = info.shape;
	if (shape.size() != 3 || shape[2] != 4)
		throw py::cast_error();

	std::array<unsigned int, 2> size{{static_cast<unsigned int>(shape[1]), static_cast<unsigned int>(shape[0])}};

	std::vector<float> data(shape[0]*shape[1]*shape[2]);
	std::copy((float*)info.ptr, (float*)info.ptr + shape[0]*shape[1]*shape[2], data.data());
	flip_vertical_inplace(data, {{static_cast<unsigned int>(shape[0]), static_cast<unsigned int>(shape[1]), static_cast<unsigned int>(shape[2])}});

	return std::make_tuple(data, size);
}

py::array_t<float> py_texture_data_to_array(const std::vector<float> &data, const std::array<unsigned int, 3> &shape)
{
	std::vector<std::size_t> dimensions{shape[0], shape[1], shape[2]};
    std::vector<std::size_t> strides{shape[1]*shape[2]*sizeof(float), shape[2]*sizeof(float), sizeof(float)};
    py::buffer_info info(const_cast<float*>(data.data()), sizeof(float), py::format_descriptor<float>::value, 3, dimensions, strides);
    return py::array_t<float>(info);
}

#endif
