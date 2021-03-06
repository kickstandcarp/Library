#define SDL_MAIN_HANDLED

#include <array>

#include "opengl.hpp"
#include "glm.hpp"
#include "opengl/window.hpp"
#include "opengl/texture.hpp"
#include "opengl/camera.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(opengl)
{
    py::module m("opengl");

    py::enum_<BlendFactor>(m, "BlendFactor")
        .value("one", BlendFactor::one)
        .value("source_alpha", BlendFactor::source_alpha)
        .value("one_minus_source_alpha", BlendFactor::one_minus_source_alpha);

    py::class_<Window>(m, "Window")
        .def("__init__", [] (Window &instance, const std::string &title, const py::iterable &py_size, const bool show) { new (&instance) Window(title, std::get<0>(iterable_to_array<unsigned int, 2>(py_size)), show); }, py::arg("title"), py::arg("size"), py::arg("show")=true)

        .def_readwrite("window_frame_buffer_name", &Window::window_frame_buffer_name)

        .def_property_readonly("size", &Window::get_size)
        .def_property_readonly("refresh_rate", &Window::get_refresh_rate)
        .def_property_readonly("refresh_time", &Window::get_refresh_time)

        .def_property("use_depth_test", &Window::get_use_depth_test, &Window::set_use_depth_test)
        .def_property("use_blending", &Window::get_use_blending, &Window::set_use_blending)
        .def_property("blend_factors", &Window::get_blend_factors, [] (Window &instance, const py::iterable &py_blend_factors) { std::array<BlendFactor, 2> blend_factors = std::get<0>(iterable_to_array<BlendFactor, 2>(py_blend_factors)); instance.set_blend_factors(std::get<0>(blend_factors), std::get<1>(blend_factors)); })
        .def_property("clear_color", &Window::get_clear_color, &Window::set_clear_color)

        .def_property_readonly("event_handler", &Window::get_event_handler)

        .def_property_readonly("shader_names", &Window::get_shader_names)
        .def("get_shader", &Window::get_shader, py::arg("name"), py::return_value_policy::reference_internal)
        
        .def_property_readonly("vertex_array_names", &Window::get_vertex_array_names)
        .def("get_vertex_array", &Window::get_vertex_array, py::arg("name"), py::return_value_policy::reference_internal)

        .def_property_readonly("texture_names", &Window::get_texture_names)
        .def("get_texture", &Window::get_texture, py::arg("name"), py::return_value_policy::reference_internal)

        .def_property_readonly("frame_buffer_names", &Window::get_frame_buffer_names)
        .def("get_frame_buffer", &Window::get_frame_buffer, py::arg("name"), py::return_value_policy::reference_internal)

        .def("set_target_frame_buffer", &Window::set_target_frame_buffer, py::arg("name"), py::arg("color_attachments")=std::vector<unsigned int>{0})

        .def("add_shader", &Window::add_shader, py::arg("name"), py::arg("shader_texts_types"), py::arg("transform_feedback_varying_names")=std::vector<std::string>())
        .def("add_vertex_array", &Window::add_vertex_array, py::arg("name"), py::arg("draw_mode"))
        .def("add_texture", [] (Window &instance, const std::string &name, const py::array_t<float> &image, TextureInterpolation interpolation, TextureWrap wrap)
        {   
            std::array<unsigned int, 2> size;
            std::vector<float> data;
            std::tie(data, size) = py_array_to_texture_data(image); 
            instance.add_texture(name, data, size, TextureFormat::rgba, interpolation, wrap);
        }, py::arg("name"), py::arg("image"), py::arg("interpolation"), py::arg("wrap"))
        .def("add_frame_buffer", [] (Window &instance, const std::string &name, const py::iterable &py_size, const std::vector<TextureFormat> &color_attachment_texture_formats, const bool has_depth_attachment) { instance.add_frame_buffer(name, std::get<0>(iterable_to_array<unsigned int, 2>(py_size)), color_attachment_texture_formats, has_depth_attachment); }, py::arg("name"), py::arg("size"), py::arg("color_attachment_texture_formats"), py::arg("has_depth_attachment"))

        .def("remove_shader", &Window::remove_shader, py::arg("name"))
        .def("remove_vertex_array", &Window::remove_vertex_array, py::arg("name"))
        .def("remove_texture", &Window::remove_vertex_array, py::arg("name"))
        .def("remove_frame_buffer", &Window::remove_frame_buffer, py::arg("name"))

        .def("clear", &Window::clear, py::arg("color")=true, py::arg("depth")=true)

        .def("draw", &Window::draw)

        .def("validate", &Window::validate);

    py::enum_<BufferUsageFrequency>(m, "BufferUsageFrequency")
        .value("stream", BufferUsageFrequency::stream)
        .value("static", BufferUsageFrequency::statical)
        .value("dynamic", BufferUsageFrequency::dynamic);

    py::enum_<BufferUsageAccess>(m, "BufferUsageAccess")
        .value("draw", BufferUsageAccess::draw)
        .value("read", BufferUsageAccess::read)
        .value("copy", BufferUsageAccess::copy);

    py::enum_<ShaderType>(m, "ShaderType")
        .value("vertex", ShaderType::vertex)
        .value("geometry", ShaderType::geometry)
        .value("fragment", ShaderType::fragment);

    py::class_<Shader>(m, "Shader")
        .def_property_readonly("attribute_names", &Shader::get_attribute_names)
        .def_property_readonly("transform_feedback_varying_names", &Shader::get_transform_feedback_varying_names)
        .def_property_readonly("uniform_names", &Shader::get_uniform_names)
        .def_property_readonly("uniform_buffer_names", &Shader::get_uniform_buffer_names)

        .def("get_uniform_type", &Shader::get_uniform_type, py::arg("name"))
        .def("get_uniform_size", &Shader::get_uniform_size, py::arg("name"))
        .def("get_uniform", [] (Shader &instance, const std::string &name, const py::handle &py_offset, const py::handle &py_size) { return py_get_uniform(instance, name, py_offset, py_size); }, py::arg("name"), py::arg("offset")=py::none(), py::arg("size")=py::none())
        
        .def("set_attribute", &Shader::set_attribute, py::arg("name"), py::arg("vertex_array"), py::arg("buffer_name"))
        .def("set_transform_feedback_varying", &Shader::set_transform_feedback_varying, py::arg("name"), py::arg("vertex_array"), py::arg("buffer_name"))
        .def("set_uniform", [] (Shader &instance, const std::string &name, const py::handle &value, const py::handle &py_offset) { py_set_uniform(instance, name, value, py_offset); }, py::arg("name"), py::arg("value"), py::arg("offset")=py::none())

        .def("initialize_uniform_buffer", &Shader::initialize_uniform_buffer, py::arg("name"), py::arg("binding"), py::arg("frequency")=BufferUsageFrequency::statical, py::arg("access")=BufferUsageAccess::draw);

    py::enum_<DrawMode>(m, "DrawMode")
        .value("points", DrawMode::points)
        .value("line_strip", DrawMode::line_strip)
        .value("line_loop", DrawMode::line_loop)
        .value("lines", DrawMode::lines)
		.value("line_strip_adjacency", DrawMode::line_strip_adjacency)
		.value("lines_adjacency", DrawMode::lines_adjacency)
		.value("triangle_strip", DrawMode::triangle_strip)
        .value("triangle_fan", DrawMode::triangle_fan)
        .value("triangles", DrawMode::triangles)
		.value("triangle_strip_adjacency", DrawMode::triangle_strip_adjacency)
		.value("triangles_adjacency", DrawMode::triangles_adjacency);

    py::class_<VertexArray>(m, "VertexArray")
        .def_property_readonly("num_vertices", &VertexArray::get_num_vertices)

        .def_readwrite("draw_mode", &VertexArray::draw_mode)

        .def_property_readonly("buffer_names", &VertexArray::get_buffer_names)
        
        .def("get_buffer_size", &VertexArray::get_buffer_size, py::arg("name"))
        .def("get_element_buffer_size", &VertexArray::get_element_buffer_size)
        .def("get_buffer_type", &VertexArray::get_buffer_type, py::arg("name"))
        .def("get_buffer_dimension", &VertexArray::get_buffer_dimension, py::arg("name"))
        .def("get_buffer_attribute_index", &VertexArray::get_buffer_attribute_index, py::arg("name"))
        .def("get_buffer_transform_feedback_varying_index", &VertexArray::get_buffer_transform_feedback_varying_index, py::arg("name"))

        .def("get_buffer", [] (VertexArray &instance, const std::string &name, const py::handle &py_offset, const py::handle &py_size) { return py_get_buffer(instance, name, py_offset, py_size); }, py::arg("name"), py::arg("offset")=py::none(), py::arg("size")=py::none())
        .def("get_element_buffer", [] (VertexArray &instance, const py::handle &py_offset, const py::handle &py_size)
        {
            unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);
            unsigned int size = py_size.is_none() ? instance.get_element_buffer_size() : py::cast<unsigned int>(py_size);
            if (size > 1)
                return py::cast<std::vector<unsigned int> >(instance.get_element_buffer(offset, size));
            else
                return py::cast<unsigned int>(instance.get_element_buffer(offset));
        }, py::arg("offset")=py::none(), py::arg("size")=py::none())

        .def("set_buffer", [] (VertexArray &instance, const std::string &name, const py::handle &py_data, const py::handle &py_offset) { py_set_buffer(instance, name, py_data, py_offset); }, py::arg("name"), py::arg("data"),  py::arg("offset")=py::none())
        .def("set_element_buffer", [] (VertexArray &instance, const py::handle &py_data, const py::handle &py_offset)
        {
            unsigned int offset = py_offset.is_none() ? 0 : py::cast<unsigned int>(py_offset);
            if (py::isinstance<py::list>(py_data) || py::isinstance<py::tuple>(py_data) || py::isinstance<py::array>(py_data))
                instance.set_element_buffer(py::cast<std::vector<unsigned int> >(py_data), offset);
            else
                instance.set_element_buffer(py::cast<unsigned int>(py_data), offset);
        }, py::arg("data"), py::arg("offset")=py::none())

        .def("add_buffer", [] (VertexArray &instance, const std::string &name, const py::iterable &py_data, const BufferUsageFrequency frequency, const BufferUsageAccess access) { return py_add_buffer(instance, name, py_data, frequency, access); }, py::arg("name"), py::arg("data"), py::arg("frequency")=BufferUsageFrequency::statical, py::arg("access")=BufferUsageAccess::draw)
        .def("add_element_buffer", [] (VertexArray &instance, const py::iterable &iterable, const BufferUsageFrequency frequency, const BufferUsageAccess access) { instance.add_element_buffer(py::cast<std::vector<unsigned int> >(iterable), frequency, access); }, py::arg("data"), py::arg("frequency")=BufferUsageFrequency::statical, py::arg("access")=BufferUsageAccess::draw)

        .def("remove_buffer", &VertexArray::remove_buffer, py::arg("name"))
        .def("remove_element_buffer", &VertexArray::remove_element_buffer)

        .def("draw", &VertexArray::draw, py::arg("count")=1, py::arg("transform_feedback")=false);

    py::enum_<TextureFormat>(m, "TextureFormat")
        .value("rgba", TextureFormat::rgba)
        .value("depth", TextureFormat::depth);

    py::enum_<TextureInterpolation>(m, "TextureInterpolation")
        .value("nearest", TextureInterpolation::nearest)
        .value("linear", TextureInterpolation::linear);

    py::enum_<TextureWrap>(m, "TextureWrap")
        .value("clamp ", TextureWrap::clamp )
        .value("mirror", TextureWrap::mirror)
        .value("repeat", TextureWrap::repeat);

    py::class_<Texture>(m, "Texture")
        .def_property_readonly("size", &Texture::get_size)

        .def("get_data", [] (Texture &instance)
        {
            std::vector<float> data = instance.get_data();
            flip_vertical_inplace(data, instance.get_shape());
            return py_texture_data_to_array(data, instance.get_shape());
        })
        
        .def("set_data", [] (Texture &instance, const py::array_t<float> &image)
        {
            std::array<unsigned int, 2> size;
            std::vector<float> data;
            std::tie(data, size) = py_array_to_texture_data(image); 
            instance.set_data(data, size);
        }, py::arg("image"))
        .def("set_texture_unit", &Texture::set_texture_unit, py::arg("index"));

    py::class_<FrameBuffer>(m, "FrameBuffer")
        .def_property_readonly("num_color_attachments", &FrameBuffer::get_num_color_attachments)
        .def_property_readonly("has_depth_attachment", &FrameBuffer::get_has_depth_attachment)

        .def("get_color_texture", &FrameBuffer::get_color_texture, py::arg("index"), py::return_value_policy::reference_internal)
        .def("get_depth_texture", &FrameBuffer::get_depth_texture, py::return_value_policy::reference_internal)

        .def("swap_color_texture", &FrameBuffer::swap_color_texture, py::arg("color_attachement_index"), py::arg("texture_name"))
        .def("copy", &FrameBuffer::copy, py::arg("color_attachment_index"));

    py::class_<Camera>(m, "Camera")
        .def(py::init<float, float, float, float, bool>(), py::arg("width"), py::arg("height"), py::arg("near_z"), py::arg("far_z"), py::arg("orthographic"))

        .def_property_readonly("coordinate_transform", (const std::shared_ptr<CoordinateTransform> (Camera::*)()) &Camera::get_coordinate_transform)
        .def_property_readonly("transform_matrix", &Camera::get_transform_matrix)

        .def("set_perspective", &Camera::set_perspective, py::arg("width"), py::arg("height"), py::arg("near_z"), py::arg("far_z"), py::arg("orthographic"));

    return m.ptr();
}
