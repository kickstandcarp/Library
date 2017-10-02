#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "glm.hpp"
#include "vector_display.hpp"
#include "vector_display_curve.hpp"
#include "trochoid.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(vector_display)
{
    py::module::import("glm");
    py::module::import("geometry");
    py::module::import("opengl");
    py::module::import("image");

    py::module m("vector_display");

	py::class_<Trochoid, Curve<glm::vec2>, std::shared_ptr<Trochoid> >(m, "Trochoid")
		.def(py::init<float, float, float, float>(), py::arg("stator_radius"), py::arg("rotor_radius"), py::arg("rotor_offset"), py::arg("min_vertex_radius"))

		.def_readwrite("stator_radius", &Trochoid::stator_radius)
		.def_readwrite("rotor_radius", &Trochoid::rotor_radius)
		.def_readwrite("rotor_offset", &Trochoid::rotor_offset);

	py::class_<VectorDisplayCurve>(m, "VectorDisplayCurve")
		.def(py::init<std::shared_ptr<Curve<glm::vec2> >, std::shared_ptr<Curve<bool> >, float, float, float, glm::vec3>(), py::arg("vertex_curve"), py::arg("drawn_curve"), py::arg("velocity"), py::arg("offset_time"),  py::arg("width"), py::arg("color"))

		.def_readwrite("velocity", &VectorDisplayCurve::velocity)
		.def_readwrite("offset_time", &VectorDisplayCurve::offset_time)
	
		.def_readwrite("width", &VectorDisplayCurve::width)
		.def_readwrite("color", &VectorDisplayCurve::color);

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

		.def("curves", &VectorDisplay::get_curve, py::arg("name"), py::return_value_policy::reference_internal)
		.def("add_curve", &VectorDisplay::add_curve, py::arg("name"), py::arg("vector_display_curve"), py::arg("window"))
		.def("remove_curve", &VectorDisplay::remove_curve, py::arg("name"), py::arg("window"))

        .def("step", &VectorDisplay::step, py::arg("elapsed_time"), py::arg("window"))
        .def("draw", &VectorDisplay::draw, py::arg("window"));

	return m.ptr();
}
