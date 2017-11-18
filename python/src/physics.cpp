#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "glm.hpp"
#include "physics/oscillator_kinetics.hpp"
#include "physics/paper_kinetics.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(physics)
{
    py::module m("physics");

    py::class_<OscillatorKinetics<float> >(m, "OscillatorKineticsFloat")
        .def(py::init<float, float, float, float>(), py::arg("value"), py::arg("velocity"), py::arg("frequency"), py::arg("damping_ratio"))

        .def_property("min_step", [] (OscillatorKinetics<float> &instance) { return instance.min_step; }, [] (OscillatorKinetics<float> &instance, const float min_step) { instance.min_step = min_step; })
        .def_property("relative_step_tolerance", [] (OscillatorKinetics<float> &instance) { return instance.relative_step_tolerance[0]; }, [] (OscillatorKinetics<float> &instance, const float relative_step_tolerance) { instance.relative_step_tolerance[0] = relative_step_tolerance; })

        .def_readwrite("frequency", &OscillatorKinetics<float>::frequency)
        .def_readwrite("damping_ratio", &OscillatorKinetics<float>::damping_ratio)
        .def_property("history_duration", [] (OscillatorKinetics<float> &instance) { return instance.history_duration; }, [] (OscillatorKinetics<float> &instance, const float history_duration) { return instance.history_duration = history_duration; })

        .def_property("value", [] (OscillatorKinetics<float> &instance) { return std::get<0>(instance.values); }, [] (OscillatorKinetics<float> &instance, const float value) { std::get<0>(instance.values) = value; })
        .def_property("velocity", [] (OscillatorKinetics<float> &instance) { return std::get<0>(instance.velocities); }, [] (OscillatorKinetics<float> &instance, const float velocity) { std::get<0>(instance.velocities) = velocity; })
        .def_property("external_acceleration", [] (OscillatorKinetics<float> &instance) { return std::get<0>(instance.external_accelerations); }, [] (OscillatorKinetics<float> &instance, const float external_acceleration) { std::get<0>(instance.external_accelerations) = external_acceleration; })

        .def_property_readonly("history", &OscillatorKinetics<float>::get_history)

        .def("add_history", &OscillatorKinetics<float>::add_history, py::arg("time"))
        .def("remove_history", &OscillatorKinetics<float>::remove_history)

        .def("steady_state_value_to_acceleration", &OscillatorKinetics<float>::steady_state_value_to_acceleration, py::arg("value"))

        .def("step", &OscillatorKinetics<float>::step, py::arg("clock"));

    py::class_<OscillatorKinetics<glm::vec2> >(m, "OscillatorKineticsVec2")
        .def(py::init<glm::vec2, glm::vec2, glm::vec2, glm::vec2>(), py::arg("value"), py::arg("velocity"), py::arg("frequency"), py::arg("damping_ratio"))

        .def_property("min_step", [] (OscillatorKinetics<glm::vec2> &instance) { return instance.min_step; }, [] (OscillatorKinetics<glm::vec2> &instance, const float min_step) { instance.min_step = min_step; })
		.def_property("relative_step_tolerance", [] (OscillatorKinetics<glm::vec2> &instance) { return instance.relative_step_tolerance[0]; }, [] (OscillatorKinetics<glm::vec2> &instance, const float x_relative_step_tolerance) { instance.relative_step_tolerance[0] = x_relative_step_tolerance; })

        .def_readwrite("frequency", &OscillatorKinetics<glm::vec2>::frequency)
        .def_readwrite("damping_ratio", &OscillatorKinetics<glm::vec2>::damping_ratio)
        .def_property("history_duration", [] (OscillatorKinetics<glm::vec2> &instance) { return instance.history_duration; }, [] (OscillatorKinetics<glm::vec2> &instance, const float history_duration) { return instance.history_duration = history_duration; })

        .def_property("value", [] (OscillatorKinetics<glm::vec2> &instance) { return std::get<0>(instance.values); }, [] (OscillatorKinetics<glm::vec2> &instance, const glm::vec2 value) { std::get<0>(instance.values) = value; })
        .def_property("velocity", [] (OscillatorKinetics<glm::vec2> &instance) { return std::get<0>(instance.velocities); }, [] (OscillatorKinetics<glm::vec2> &instance, const glm::vec2 velocity) { std::get<0>(instance.velocities) = velocity; })
        .def_property("external_acceleration", [] (OscillatorKinetics<glm::vec2> &instance) { return std::get<0>(instance.external_accelerations); }, [] (OscillatorKinetics<glm::vec2> &instance, const glm::vec2 external_acceleration) { std::get<0>(instance.external_accelerations) = external_acceleration; })

        .def_property_readonly("history", &OscillatorKinetics<glm::vec2>::get_history)
        
        .def("add_history", &OscillatorKinetics<glm::vec2>::add_history, py::arg("time"))
        .def("remove_history", &OscillatorKinetics<glm::vec2>::remove_history)

        .def("steady_state_value_to_acceleration", &OscillatorKinetics<glm::vec2>::steady_state_value_to_acceleration, py::arg("value"))

        .def("step", &OscillatorKinetics<glm::vec2>::step, py::arg("clock"));

    py::class_<PaperKinetics>(m, "PaperKinetics")
        .def(py::init<glm::vec3, glm::quat, glm::vec3, glm::vec3, float, float, float, float, glm::vec3>(), py::arg("position"), py::arg("orientation"), py::arg("velocity"), py::arg("angular_velocity"), py::arg("perpendicular_friction"), py::arg("parallel_friction"), py::arg("fluid_density"), py::arg("paper_density"), py::arg("size"))

        .def_property("min_step", [] (PaperKinetics &instance) { return instance.min_step; }, [] (PaperKinetics &instance, const float min_step) { instance.min_step = min_step; })
        .def_property("relative_step_tolerance", [] (PaperKinetics &instance) { return instance.relative_step_tolerance; }, [] (PaperKinetics &instance, const py::iterable &iterable) { instance.relative_step_tolerance = std::get<0>(iterable_to_array<float, 2>(iterable)); })

        .def_readwrite("perpendicular_friction", &PaperKinetics::perpendicular_friction)
        .def_readwrite("parallel_friction", &PaperKinetics::parallel_friction)
        .def_readwrite("fluid_density", &PaperKinetics::fluid_density)
        .def_readwrite("paper_density", &PaperKinetics::paper_density)
        .def_readwrite("size", &PaperKinetics::size)
        .def_property("history_duration", [] (PaperKinetics &instance) { return instance.history_duration; }, [] (PaperKinetics &instance, const float history_duration) { return instance.history_duration = history_duration; })

        .def_property("position", [] (PaperKinetics &instance) { return std::get<0>(instance.values); }, [] (PaperKinetics &instance, const glm::vec3 &value) { std::get<0>(instance.values) = value; })
        .def_property("orientation", [] (PaperKinetics &instance) { return std::get<1>(instance.values); }, [] (PaperKinetics &instance, const glm::quat &value) { std::get<1>(instance.values) = value; })
        .def_property("velocity", [] (PaperKinetics &instance) { return std::get<0>(instance.velocities); }, [] (PaperKinetics &instance, const glm::vec3 &velocity) { std::get<0>(instance.velocities) = velocity; })
        .def_property("angular_velocity", [] (PaperKinetics &instance) { return glm::vec3(std::get<1>(instance.velocities).x, std::get<1>(instance.velocities).y, std::get<1>(instance.velocities).z); }, [] (PaperKinetics &instance, const glm::vec3 &angular_velocity) { std::get<1>(instance.velocities) = glm::quat(0.0f, angular_velocity.x, angular_velocity.y, angular_velocity.z); })
        .def_property("external_acceleration", [] (PaperKinetics &instance) { return std::get<0>(instance.external_accelerations); }, [] (PaperKinetics &instance, const glm::vec3 &external_acceleration) { std::get<0>(instance.external_accelerations) = external_acceleration; })
        .def_property("external_angular_acceleration", [] (PaperKinetics &instance) { return std::get<1>(instance.external_accelerations); }, [] (PaperKinetics &instance, const glm::vec3 &external_angular_acceleration) { std::get<1>(instance.external_accelerations) = glm::quat(0.0f, external_angular_acceleration.x, external_angular_acceleration.y, external_angular_acceleration.z); })

        .def_property_readonly("position_history", &PaperKinetics::get_position_history)
        .def_property_readonly("orienation_history", &PaperKinetics::get_orientation_history)

        .def("add_position_history", &PaperKinetics::add_position_history, py::arg("time"))
        .def("add_orientation_history", &PaperKinetics::add_orientation_history, py::arg("time"))

        .def("remove_position_history", &PaperKinetics::remove_position_history)
        .def("remove_orientation_history", &PaperKinetics::remove_orientation_history)

        .def("step", &PaperKinetics::step, py::arg("clock"));

    return m.ptr();
}
