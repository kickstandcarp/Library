#include <pybind11/pybind11.h>
#include <glm/glm.hpp>

#include "event/event_handler.hpp"
#include "repr/event_repr.hpp"

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

		.def("get_button", &EventHandler::get_button, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_button_moved_down", &EventHandler::get_button_moved_down, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_button_moved_up", &EventHandler::get_button_moved_up, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_value", &EventHandler::get_value, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_direction", &EventHandler::get_direction, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

		.def("update", &EventHandler::update)

		.def("__repr__", &event_handler_repr);

	return m.ptr();
}
