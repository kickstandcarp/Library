#include <glm/glm.hpp>
#include <pybind11/pybind11.h>

#include "event/event_handler.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(event)
{
    py::module m("event");

    py::enum_<DeviceType>(m, "DeviceType")
        .value("keyboard", DeviceType::keyboard)
        .value("mouse", DeviceType::mouse)
        .value("controller", DeviceType::controller);

    py::class_<EventHandler>(m, "EventHandler")
        .def_property_readonly("num_controllers", &EventHandler::get_num_controllers)

        .def_readwrite("quit", &EventHandler::quit)

        .def_readwrite("button_direction_delimiter", &EventHandler::button_direction_delimiter)

        .def_readwrite("stick_threshold", &EventHandler::stick_threshold)
        .def_readwrite("trigger_threshold", &EventHandler::trigger_threshold)

        .def_readwrite("history_duration", &EventHandler::history_duration)

        .def("get_button", &EventHandler::get_button, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_button_moved_down", &EventHandler::get_button_moved_down, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_button_moved_up", &EventHandler::get_button_moved_up, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_value", &EventHandler::get_value, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_direction", &EventHandler::get_direction, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

        .def("get_button_history", &EventHandler::get_button_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_value_history", &EventHandler::get_value_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("get_direction_history", &EventHandler::get_direction_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

        .def("add_button_history", &EventHandler::add_button_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)
        .def("add_value_history", &EventHandler::add_value_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)
        .def("add_direction_history", &EventHandler::add_direction_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)

        .def("remove_button_history", &EventHandler::get_button_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("remove_value_history", &EventHandler::get_value_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
        .def("remove_direction_history", &EventHandler::get_direction_history, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

        .def("step", &EventHandler::step, py::arg("clock"));

    return m.ptr();
}
