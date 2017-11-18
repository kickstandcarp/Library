#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "glm.hpp"
#include "vector_display.hpp"
#include "vector_display_beam.hpp"
#include "trochoid.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(vector_display)
{
    py::module m("vector_display");

    py::class_<Trochoid, Curve<glm::vec2>, std::shared_ptr<Trochoid> >(m, "Trochoid")
        .def(py::init<float, float, float, float>(), py::arg("stator_radius"), py::arg("rotor_radius"), py::arg("rotor_offset"), py::arg("min_vertex_radius"))

        .def_readwrite("stator_radius", &Trochoid::stator_radius)
        .def_readwrite("rotor_radius", &Trochoid::rotor_radius)
        .def_readwrite("rotor_offset", &Trochoid::rotor_offset);

    py::class_<VectorDisplayBeam>(m, "VectorDisplayBeam")
        .def(py::init<std::shared_ptr<Curve<glm::vec2> >, glm::vec3, float, float, float>(), py::arg("vertex_curve"), py::arg("color"), py::arg("length"), py::arg("velocity"), py::arg("t")=0.0f)
        .def(py::init<std::shared_ptr<Curve<glm::vec2> >, std::shared_ptr<Curve<bool> >, glm::vec3, float, float, float>(), py::arg("vertex_curve"), py::arg("activation_curve"), py::arg("color"), py::arg("length"), py::arg("velocity"), py::arg("t")=0.0f)
        .def(py::init<std::shared_ptr<Curve<glm::vec2> >, std::shared_ptr<Curve<float> >, glm::vec3, float, float, float>(), py::arg("vertex_curve"), py::arg("excitation_curve"), py::arg("color"), py::arg("length"), py::arg("velocity"), py::arg("t")=0.0f)

        .def_readwrite("color", &VectorDisplayBeam::color)
        .def_readwrite("excitation", &VectorDisplayBeam::excitation)
        .def_readwrite("radius", &VectorDisplayBeam::radius)
        .def_readwrite("velocity", &VectorDisplayBeam::velocity)

        .def_readwrite("excitation_time_constant", &VectorDisplayBeam::excitation_time_constant)
        .def_readwrite("excitation_decay_time_constant", &VectorDisplayBeam::excitation_decay_time_constant)
        .def_readwrite("decay_time_constant", &VectorDisplayBeam::decay_time_constant)

        .def_readwrite("vertex_curve", &VectorDisplayBeam::vertex_curve)
        .def_readwrite("activation_curve", &VectorDisplayBeam::activation_curve)
        .def_readwrite("excitation_curve", &VectorDisplayBeam::excitation_curve)

        .def_readwrite("t", &VectorDisplayBeam::t);

    py::class_<VectorDisplay>(m, "VectorDisplay")
        .def("__init__", [] (VectorDisplay &instance, Window &window, const py::iterable &py_size) { new (&instance) VectorDisplay(window, std::get<0>(iterable_to_array<unsigned int, 2>(py_size))); }, py::arg("window"), py::arg("size"))

        .def_readwrite("threshold", &VectorDisplay::threshold)

        .def_readonly("glow_filter", &VectorDisplay::glow_filter)

        .def_property_readonly("beam_names", &VectorDisplay::get_beam_names)
        .def("beams", &VectorDisplay::get_beam, py::arg("name"), py::return_value_policy::reference_internal)
        .def("add_beam", &VectorDisplay::add_beam, py::arg("name"), py::arg("vector_display_beam"), py::arg("window"))
        .def("remove_beam", &VectorDisplay::remove_beam, py::arg("name"), py::arg("window"))

        .def("step", &VectorDisplay::step, py::arg("clock"), py::arg("window"), py::arg("beam_names")=std::vector<std::string>())

        .def("draw", &VectorDisplay::draw, py::arg("window"));

    return m.ptr();
}
