#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "glm.hpp"
#include "vector_display.hpp"
#include "vector_display_path.hpp"
#include "trochoid_path.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(vector_display)
{
    py::module::import("glm");
    py::module::import("geometry");
    py::module::import("opengl");
    py::module::import("image");

    py::module m("vector_display");

	py::class_<TrochoidPath, Path<glm::vec2>, std::shared_ptr<TrochoidPath> >(m, "TrochoidPath")
		.def(py::init<float, float, float, float>(), py::arg("stator_radius"), py::arg("rotor_radius"), py::arg("rotor_offset"), py::arg("min_vertex_radius"))

		.def_readwrite("stator_radius", &TrochoidPath::stator_radius)
		.def_readwrite("rotor_radius", &TrochoidPath::rotor_radius)
		.def_readwrite("rotor_offset", &TrochoidPath::rotor_offset);

	py::class_<VectorDisplayPath>(m, "VectorDisplayPath")
		.def(py::init<std::shared_ptr<Path<glm::vec2> >, std::shared_ptr<Path<bool> >, float, float, float, glm::vec3>(), py::arg("vertex_path"), py::arg("drawn_path"), py::arg("t"), py::arg("width"), py::arg("velocity"), py::arg("color"))

		.def_readwrite("width", &VectorDisplayPath::width)
		.def_readwrite("velocity", &VectorDisplayPath::velocity)
		.def_readwrite("color", &VectorDisplayPath::color)

		.def("pop_back", &VectorDisplayPath::pop_back);

	py::class_<VectorDisplay>(m, "VectorDisplay")
		.def("__init__", [] (VectorDisplay &instance, Window &window, const py::iterable &py_size, const std::string &frame_buffer_name) { new (&instance) VectorDisplay(window, std::get<0>(iterable_to_array<unsigned int, 2>(py_size)), frame_buffer_name); }, py::arg("window"), py::arg("size"), py::arg("frame_buffer_name"))

		.def_readwrite("threshold", &VectorDisplay::threshold)
		
		.def_readwrite("excitation_time_constant", &VectorDisplay::excitation_time_constant)
        .def_readwrite("decay_time_constant_1", &VectorDisplay::decay_time_constant_1)
		.def_readwrite("decay_time_constant_2", &VectorDisplay::decay_time_constant_2)
		.def_readwrite("decay_time_constant_edge_1", &VectorDisplay::decay_time_constant_edge_1)
		.def_readwrite("decay_time_constant_edge_2", &VectorDisplay::decay_time_constant_edge_2)

		.def_readonly("kinetics", &VectorDisplay::kinetics)
		.def_readonly("beam_kinetics", &VectorDisplay::beam_kinetics)

		.def_readonly("glow_filter", &VectorDisplay::glow_filter)

		.def("paths", &VectorDisplay::get_path, py::arg("name"))
		.def("add_path", &VectorDisplay::add_path, py::arg("name"), py::arg("vector_display_path"))
		.def("remove_path", &VectorDisplay::remove_path, py::arg("name"))

        .def("step", &VectorDisplay::step, py::arg("elapsed_time"), py::arg("window"))
        .def("draw", &VectorDisplay::draw, py::arg("window"));

	return m.ptr();
}
