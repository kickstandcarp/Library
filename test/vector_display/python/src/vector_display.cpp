#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "vector_display.hpp"
#include "vector_display_path.hpp"
#include "trochoid_path.hpp"
#include "glm.hpp"
#include "opengl/window.hpp"
#include "geometry/path.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(vector_display)
{
    py::module m("vector_display");

	py::class_<TrochoidPath, Path<glm::vec2>, std::shared_ptr<TrochoidPath> >(m, "TrochoidPath")
		.def(py::init<float, float, float, float>(), py::arg("stator_radius"), py::arg("rotor_radius"), py::arg("rotor_offset"), py::arg("min_vertex_radius"))

		.def_readwrite("stator_radius", &TrochoidPath::stator_radius)
		.def_readwrite("rotor_radius", &TrochoidPath::rotor_radius)
		.def_readwrite("rotor_offset", &TrochoidPath::rotor_offset);

	py::class_<VectorDisplayPath>(m, "VectorDisplayPath")
		.def(py::init<std::shared_ptr<Path<glm::vec2> >, float, float, float, glm::vec3>(), py::arg("path"), py::arg("t"), py::arg("width"), py::arg("velocity"), py::arg("color"))

		.def_readwrite("width", &VectorDisplayPath::width)
		.def_readwrite("velocity", &VectorDisplayPath::velocity)
		.def_readwrite("color", &VectorDisplayPath::color)

		.def("pop_vertices", &VectorDisplayPath::pop_vertices);

	py::class_<VectorDisplay>(m, "VectorDisplay")
		.def("__init__", [] (VectorDisplay &instance, Window &window, const py::iterable &py_size) { new (&instance) VectorDisplay(window, std::get<0>(iterable_to_array<unsigned int, 2>(py_size))); }, py::arg("window"), py::arg("size"))

        .def_readwrite("excitation_time_constant", &VectorDisplay::excitation_time_constant)
        .def_readwrite("decay_time_constant_1", &VectorDisplay::decay_time_constant_1)
        .def_readwrite("decay_time_constant_2", &VectorDisplay::decay_time_constant_2)
        .def_readwrite("decay_threshold_1", &VectorDisplay::decay_threshold_1)
        .def_readwrite("decay_threshold_2", &VectorDisplay::decay_threshold_2)

		.def_readwrite("kinetics", &VectorDisplay::kinetics)
		.def_readwrite("beam_kinetics", &VectorDisplay::beam_kinetics)

		.def("paths", &VectorDisplay::get_path, py::arg("name"))
		.def("add_path", &VectorDisplay::add_path, py::arg("name"), py::arg("vector_display_path"))
		.def("remove_path", &VectorDisplay::remove_path, py::arg("name"))

        .def("update", &VectorDisplay::update, py::arg("elapsed_time"))
        .def("draw", &VectorDisplay::draw, py::arg("window"));

	return m.ptr();
}
