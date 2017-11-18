#ifndef PYTHON_OPENGL_HPP
#define PYTHON_OPENGL_HPP

#include <stdexcept>
#include <utility>
#include <limits>
#include <string>
#include <tuple>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"
#include "opengl/buffer_support.hpp"
#include "image/transform.hpp"

namespace py = pybind11;

py::object                                                          py_get_uniform(const Shader &shader, const std::string &name, const py::handle &py_offset, const py::handle &py_size);
void                                                                py_set_uniform(Shader &shader, const std::string &name, const py::handle &py_value, const py::handle &py_offset);

py::object                                                          py_get_buffer(const VertexArray &vertex_array, const std::string &name, const py::handle &py_offset, const py::handle &py_size);
void                                                                py_set_buffer(VertexArray &vertex_array, const std::string &name, const py::handle &py_data, const py::handle &py_offset);
void                                                                py_add_buffer(VertexArray &vertex_array, const std::string &name, const py::iterable &py_data, const BufferUsageFrequency frequency, const BufferUsageAccess access);
template <class T> bool                                             py_add_buffer_type(VertexArray &vertex_array, const std::string &name, const py::iterable &data, const BufferUsageFrequency frequency, const BufferUsageAccess access);

std::tuple<std::vector<float>, const std::array<unsigned int, 2> >  py_array_to_texture_data(const py::array_t<float> &image);
py::array_t<float>                                                  py_texture_data_to_array(const std::vector<float> &data, const std::array<unsigned int, 3> &shape);



py::object py_get_uniform(const Shader &shader, const std::string &name, const py::handle &py_offset, const py::handle &py_size)
{
    unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);
    unsigned int size = py_size.is_none() ? shader.get_uniform_size(name) : py::cast<unsigned int>(py_size);

    bool return_vector = shader.get_uniform_size(name) > 1 && size > 1;

    switch (shader.get_uniform_type(name))
    {
        case GL_INT:
            if (return_vector)
                return py::cast<std::vector<int> >(shader.get_uniform<int>(name, offset, size));
            else
                return py::cast<int>(shader.get_uniform<int>(name, offset));
            break;
        case GL_INT_VEC2:
            if (return_vector)
                return py::cast<std::vector<glm::ivec2> >(shader.get_uniform<glm::ivec2>(name, offset, size));
            else
                return py::cast<glm::ivec2>(shader.get_uniform<glm::ivec2>(name, offset));
            break;
        case GL_INT_VEC3:
            if (return_vector)
                return py::cast<std::vector<glm::ivec3> >(shader.get_uniform<glm::ivec3>(name, offset, size));
            else
                return py::cast<glm::ivec3>(shader.get_uniform<glm::ivec3>(name, offset));
            break;
        case GL_INT_VEC4:
            if (return_vector)
                return py::cast<std::vector<glm::ivec4> >(shader.get_uniform<glm::ivec4>(name, offset, size));
            else
                return py::cast<glm::ivec4>(shader.get_uniform<glm::ivec4>(name, offset));
            break;
        case GL_FLOAT:
            if (return_vector)
                return py::cast<std::vector<float> >(shader.get_uniform<float>(name, offset, size));
            else
                return py::cast<float>(shader.get_uniform<float>(name, offset));
            break;
        case GL_FLOAT_VEC2:
            if (return_vector)
                return py::cast<std::vector<glm::vec2> >(shader.get_uniform<glm::vec2>(name, offset, size));
            else
                return py::cast<glm::vec2>(shader.get_uniform<glm::vec2>(name, offset));
            break;
        case GL_FLOAT_VEC3:
            if (return_vector)
                return py::cast<std::vector<glm::vec3> >(shader.get_uniform<glm::vec3>(name, offset, size));
            else
                return py::cast<glm::vec3>(shader.get_uniform<glm::vec3>(name, offset));
            break;
        case GL_FLOAT_VEC4:
            if (return_vector)
                return py::cast<std::vector<glm::vec4> >(shader.get_uniform<glm::vec4>(name, offset, size));
            else
                return py::cast<glm::vec4>(shader.get_uniform<glm::vec4>(name, offset));
            break;
        case GL_FLOAT_MAT4:
            if (return_vector)
                return py::cast<std::vector<glm::mat4x4> >(shader.get_uniform<glm::mat4x4>(name, offset, size));
            else
                return py::cast<glm::mat4x4>(shader.get_uniform<glm::mat4x4>(name, offset));
            break;
        case GL_SAMPLER_2D:
            if (return_vector)
                return py::cast<std::vector<int> >(shader.get_uniform<int>(name, offset, size));
            else
                return py::cast<int>(shader.get_uniform<int>(name, offset));
            break;
    }

    throw std::invalid_argument("unsupported uniform type");
}

void py_set_uniform(Shader &shader, const std::string &name, const py::handle &py_value, const py::handle &py_offset)
{
    unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);

    bool argument_vector = py::isinstance<py::list>(py_value) || py::isinstance<py::tuple>(py_value) || py::isinstance<py::array>(py_value);

    switch (shader.get_uniform_type(name))
    {
        case GL_INT:
            if (argument_vector)
                shader.set_uniform<int>(name, py::cast<std::vector<int> >(py_value), offset);
            else
                shader.set_uniform<int>(name, py::cast<int>(py_value), offset);
            return;
            break;
        case GL_INT_VEC2:
            if (argument_vector)
                shader.set_uniform<glm::ivec2>(name, py::cast<std::vector<glm::ivec2> >(py_value), offset);
            else
                shader.set_uniform<glm::ivec2>(name, py::cast<glm::ivec2>(py_value), offset);
            return;
            break;
        case GL_INT_VEC3:
            if (argument_vector)
                shader.set_uniform<glm::ivec3>(name, py::cast<std::vector<glm::ivec3> >(py_value), offset);
            else
                shader.set_uniform<glm::ivec3>(name, py::cast<glm::ivec3>(py_value), offset);
            return;
            break;
        case GL_INT_VEC4:
            if (argument_vector)
                shader.set_uniform<glm::ivec4>(name, py::cast<std::vector<glm::ivec4> >(py_value), offset);
            else
                shader.set_uniform<glm::ivec4>(name, py::cast<glm::ivec4>(py_value), offset);
            break;
        case GL_FLOAT:
            if (argument_vector)
                shader.set_uniform<float>(name, py::cast<std::vector<float> >(py_value), offset);
            else
                shader.set_uniform<float>(name, py::cast<float>(py_value), offset);
            return;
            break;
        case GL_FLOAT_VEC2:
            if (argument_vector)
                shader.set_uniform<glm::vec2>(name, py::cast<std::vector<glm::vec2> >(py_value), offset);
            else
                shader.set_uniform<glm::vec2>(name, py::cast<glm::vec2>(py_value), offset);
            return;
            break;
        case GL_FLOAT_VEC3:
            if (argument_vector)
                shader.set_uniform<glm::vec3>(name, py::cast<std::vector<glm::vec3> >(py_value), offset);
            else
                shader.set_uniform<glm::vec3>(name, py::cast<glm::vec3>(py_value), offset);
            return;
            break;
        case GL_FLOAT_VEC4:
            if (argument_vector)
                shader.set_uniform<glm::vec4>(name, py::cast<std::vector<glm::vec4> >(py_value), offset);
            else
                shader.set_uniform<glm::vec4>(name, py::cast<glm::vec4>(py_value), offset);
            return;
            break;
        case GL_FLOAT_MAT4:
            if (argument_vector)
                shader.set_uniform<glm::mat4x4>(name, py::cast<std::vector<glm::mat4x4> >(py_value), offset);
            else
                shader.set_uniform<glm::mat4x4>(name, py::cast<glm::mat4x4>(py_value), offset);
            return;
            break;
        case GL_SAMPLER_2D:
            if (argument_vector)
                shader.set_uniform<int>(name, py::cast<std::vector<int> >(py_value), offset);
            else
                shader.set_uniform<int>(name, py::cast<int>(py_value), offset);
            return;
            break;
    }

    throw std::invalid_argument("unsupported uniform type");
}

py::object py_get_buffer(const VertexArray &vertex_array, const std::string &name, const py::handle &py_offset, const py::handle &py_size)
{
    unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);
    unsigned int size = py_size.is_none() ? vertex_array.get_buffer_size(name) : py::cast<unsigned int>(py_size);
    unsigned int dimension = vertex_array.get_buffer_dimension(name);

    bool return_vector = size > 1;

    switch (vertex_array.get_buffer_type(name))
    {
        case GL_INT:
            if (dimension == 1)
            {
                if (return_vector)
                    return py::cast<std::vector<int> >(vertex_array.get_buffer<int>(name, offset, size));
                else
                    return py::cast<int>(vertex_array.get_buffer<int>(name, offset));
            }
            else if (dimension == 2)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::ivec2> >(vertex_array.get_buffer<glm::ivec2>(name, offset, size));
                else
                    return py::cast<glm::ivec2>(vertex_array.get_buffer<glm::ivec2>(name, offset));
            }
            else if (dimension == 3)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::ivec3> >(vertex_array.get_buffer<glm::ivec3>(name, offset, size));
                else
                    return py::cast<glm::ivec3>(vertex_array.get_buffer<glm::ivec3>(name, offset));
            }
            else if (dimension == 4)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::ivec4> >(vertex_array.get_buffer<glm::ivec4>(name, offset, size));
                else
                    return py::cast<glm::ivec4>(vertex_array.get_buffer<glm::ivec4>(name, offset));
            }
            break;
        case GL_UNSIGNED_INT:
            if (dimension == 1)
            {
                if (return_vector)
                    return py::cast<std::vector<unsigned int> >(vertex_array.get_buffer<unsigned int>(name, offset, size));
                else
                    return py::cast<unsigned int>(vertex_array.get_buffer<unsigned int>(name, offset));
            }
            break;
        case GL_FLOAT:
            if (dimension == 1)
            {
                if (return_vector)
                    return py::cast<std::vector<float> >(vertex_array.get_buffer<float>(name, offset, size));
                else
                    return py::cast<float>(vertex_array.get_buffer<float>(name, offset));
            }
            else if (dimension == 2)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::vec2> >(vertex_array.get_buffer<glm::vec2>(name, offset, size));
                else
                    return py::cast<glm::vec2>(vertex_array.get_buffer<glm::vec2>(name, offset));
            }
            else if (dimension == 3)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::vec3> >(vertex_array.get_buffer<glm::vec3>(name, offset, size));
                else
                    return py::cast<glm::vec3>(vertex_array.get_buffer<glm::vec3>(name, offset));
            }
            else if (dimension == 4)
            {
                if (return_vector)
                    return py::cast<std::vector<glm::vec4> >(vertex_array.get_buffer<glm::vec4>(name, offset, size));
                else
                    return py::cast<glm::vec4>(vertex_array.get_buffer<glm::vec4>(name, offset));
            }
            break;
    }
    
    throw std::invalid_argument("unsupported buffer type");
}

void py_set_buffer(VertexArray &vertex_array, const std::string &name, const py::handle &py_data, const py::handle &py_offset)
{
    unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);
    unsigned int dimension = vertex_array.get_buffer_dimension(name);

    bool argument_vector = py::isinstance<py::list>(py_data) || py::isinstance<py::tuple>(py_data) || py::isinstance<py::array>(py_data);

    switch (vertex_array.get_buffer_type(name))
    {
        case GL_INT:
            if (dimension == 1)
            {
                if (argument_vector)
                    vertex_array.set_buffer<int>(name, py::cast<std::vector<int> >(py_data), offset);
                else
                    vertex_array.set_buffer<int>(name, py::cast<int>(py_data), offset);
                return;
            }
            else if (dimension == 2)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::ivec2>(name, py::cast<std::vector<glm::ivec2> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::ivec2>(name, py::cast<glm::ivec2>(py_data), offset);
                return;
            }
            else if (dimension == 3)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::ivec3>(name, py::cast<std::vector<glm::ivec3> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::ivec3>(name, py::cast<glm::ivec3>(py_data), offset);
                return;
            }
            else if (dimension == 4)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::ivec4>(name, py::cast<std::vector<glm::ivec4> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::ivec4>(name, py::cast<glm::ivec4>(py_data), offset);
                return;
            }
            break;
        case GL_UNSIGNED_INT:
            if (dimension == 1)
            {
                if (argument_vector)
                    vertex_array.set_buffer<unsigned int>(name, py::cast<std::vector<unsigned int> >(py_data), offset);
                else
                    vertex_array.set_buffer<unsigned int>(name, py::cast<unsigned int>(py_data), offset);
                return;
            }
            break;
        case GL_FLOAT:
            if (dimension == 1)
            {
                if (argument_vector)
                    vertex_array.set_buffer<float>(name, py::cast<std::vector<float> >(py_data), offset);
                else
                    vertex_array.set_buffer<float>(name, py::cast<float>(py_data), offset);
                return;
            }
            else if (dimension == 2)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::vec2>(name, py::cast<std::vector<glm::vec2> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::vec2>(name, py::cast<glm::vec2>(py_data), offset);
                return;
            }
            else if (dimension == 3)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::vec3>(name, py::cast<std::vector<glm::vec3> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::vec3>(name, py::cast<glm::vec3>(py_data), offset);
                return;
            }
            else if (dimension == 4)
            {
                if (argument_vector)
                    vertex_array.set_buffer<glm::vec4>(name, py::cast<std::vector<glm::vec4> >(py_data), offset);
                else
                    vertex_array.set_buffer<glm::vec4>(name, py::cast<glm::vec4>(py_data), offset);
                return;
            }
            break;
    }

    throw std::invalid_argument("unsupported buffer type");
}

void py_add_buffer(VertexArray &vertex_array, const std::string &name, const py::iterable &py_data, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    if (py_add_buffer_type<int>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::ivec2>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::ivec3>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::ivec4>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<unsigned int>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<float>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::vec2>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::vec3>(vertex_array, name, py_data, frequency, access))
        return;
    if (py_add_buffer_type<glm::vec4>(vertex_array, name, py_data, frequency, access))
        return;

    throw std::invalid_argument("unsupported buffer type");
}

template <class T>
bool py_add_buffer_type(VertexArray &vertex_array, const std::string &name, const py::iterable &py_data, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    try
    {
        vertex_array.add_buffer<T>(name, py::cast<std::vector<T> >(py_data), frequency, access);
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
