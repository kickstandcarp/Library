#include <pybind11/pybind11.h>
#include <glm/glm.hpp>

#include "physics/oscillator_kinetics.hpp"
#include "physics/paper_kinetics.hpp"
#include "repr/physics_repr.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(physics)
{
    py::module m("physics");

    py::class_<OscillatorKinetics>(m, "OscillatorKinetics")
        .def(py::init<float, float, float, float, float>(), py::arg("value"), py::arg("velocity"), py::arg("frequency"), py::arg("damping_ratio"), py::arg("acceleration"))

        .def_readwrite("frequency", &OscillatorKinetics::frequency)
        .def_readwrite("damping_ratio", &OscillatorKinetics::damping_ratio)
		.def_readwrite("acceleration", &OscillatorKinetics::acceleration)
		.def_readwrite("value", &OscillatorKinetics::value)
		.def_readwrite("velocity", &OscillatorKinetics::velocity)

        .def("steady_state_value_acceleration", &OscillatorKinetics::steady_state_value_acceleration, py::arg("value"))

		.def("step", &OscillatorKinetics::step, py::arg("elapsed_time"))

		.def("__repr__", &oscillator_kinetics_repr);

    py::class_<PaperKinetics>(m, "PaperKinetics")
        .def(py::init<glm::vec3, glm::quat, glm::vec3, glm::vec3, float, float, float, float, glm::vec3, glm::vec3>(), py::arg("position")=glm::vec3(0.0f), py::arg("orientation")=glm::quat(1.0f, 0.0f, 0.0f, 0.0f), py::arg("velocity")=glm::vec3(0.0f), py::arg("angular_velocity")=glm::vec3(0.0f), py::arg("perpendicular_friction")=10.0f, py::arg("parallel_friction")=0.1f, py::arg("fluid_density")=0.1f, py::arg("paper_density")=1.0f, py::arg("size")=glm::vec3(1.0f, 0.0f, 1.0f), py::arg("acceleration")=glm::vec3(0.0f, -9.8f, 0.0f))

		.def_readwrite("perpendicular_friction", &PaperKinetics::perpendicular_friction)
		.def_readwrite("parallel_friction", &PaperKinetics::parallel_friction)
		.def_readwrite("fluid_density", &PaperKinetics::fluid_density)
		.def_readwrite("paper_density", &PaperKinetics::paper_density)
		.def_readwrite("size", &PaperKinetics::size)
		.def_readwrite("acceleration", &PaperKinetics::acceleration)
		.def_readwrite("position", &PaperKinetics::position)
		.def_readwrite("orientation", &PaperKinetics::orientation)
		.def_readwrite("velocity", &PaperKinetics::velocity)
		.def_readwrite("angular_velocity", &PaperKinetics::angular_velocity)

		.def("step", &PaperKinetics::step, py::arg("elapsed_time"))

		.def("__repr__", &paper_kinetics_repr);

	return m.ptr();
}
