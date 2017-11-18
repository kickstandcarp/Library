#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "vector_display_curve.hpp"
#include "background.hpp"
#include "square.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(explode_mode)
{
    py::module m("explode_mode");

    py::class_<VectorDisplayCurve>(m, "VectorDisplayCurve")
		.def(py::init<std::vector<glm::vec4>&, Window&, std::string>(), py::arg("vertices"), py::arg("window"), py::arg("vertex_array_name"))

		.def_property_readonly("first_vertex", &VectorDisplayCurve::get_first_vertex)
		.def_property_readonly("last_vertex", &VectorDisplayCurve::get_last_vertex)

		.def_property_readonly("vertex_array_name", &VectorDisplayCurve::get_vertex_array_name)
   
		.def_readwrite("start_time", &VectorDisplayCurve::start_time)
		.def_readwrite("velocity", &VectorDisplayCurve::velocity)

		.def_readwrite("color", &VectorDisplayCurve::color)
		.def_readwrite("excitation_duration", &VectorDisplayCurve::excitation_duration)
		.def_readwrite("excitation", &VectorDisplayCurve::excitation)
		.def_readwrite("excitation_time_constant", &VectorDisplayCurve::excitation_time_constant)
		.def_readwrite("decay_time_constant", &VectorDisplayCurve::decay_time_constant)

		.def_readwrite("translation", &VectorDisplayCurve::translation)
		.def_readwrite("rotation", &VectorDisplayCurve::rotation)
		.def_readwrite("width", &VectorDisplayCurve::width)
		.def_readwrite("cap_height", &VectorDisplayCurve::cap_height)

		.def("step", &VectorDisplayCurve::step, py::arg("clock"), py::arg("window"))
		.def("draw", &VectorDisplayCurve::draw, py::arg("window"));

	m.def("initialize_window_vector_display_curve", &initialize_window_vector_display_curve, py::arg("window"), py::arg("step_shader_name")="vector_display_curve_step", py::arg("draw_shader_name")="vector_display_curve_draw");

    py::class_<Background>(m, "Background")
        .def(py::init<float, float, unsigned int, Window&, std::string>(), py::arg("radius"), py::arg("height"), py::arg("num_vertices"), py::arg("window"), py::arg("vertex_array_name"))

        .def_property_readonly("vertex_array_name", &Background::get_vertex_array_name)

        .def("step", &Background::step, py::arg("clock"), py::arg("window"))
        .def("draw", &Background::draw, py::arg("camera"), py::arg("window"));

    m.def("initialize_window_background", &initialize_window_background, py::arg("window"), py::arg("step_shader_name")="background_step", py::arg("draw_shader_name")="background_draw");

	py::class_<Square>(m, "Square")
		.def(py::init<glm::vec3, glm::quat, glm::vec3, glm::vec3>(), py::arg("position"), py::arg("orientation"), py::arg("velocity"), py::arg("angular_velocity"))

		.def_readwrite("coordinate_transform", &Square::coordinate_transform)
		.def_readwrite("kinetics", &Square::kinetics)

		.def("dissolve", &Square::dissolve)

		.def("step", &Square::step, py::arg("clock"))
		.def("draw", &Square::draw, py::arg("camera"), py::arg("window"));

	m.def("initialize_window_square", &initialize_window_square, py::arg("window"), py::arg("shader_name")="square", py::arg("vertex_array_name")="square");

	/* py::class_<Player>(m, "Player");
		.def(py::init<glm::vec3, glm::quat, glm::vec3, glm::vec3>(), py::arg("position"), py::arg("orientation"), py::arg("velocity"), py::arg("angular_velocity"))

		.def("update", &Player::update, py::arg("event_handler"))
		.def("step", &Player::step, py::arg("clock"))
		.def("draw", &Player::draw, py::arg("camera"), py::arg("window")); */

    return m.ptr();
}
