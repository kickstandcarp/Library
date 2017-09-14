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

		.def_readwrite("path_duration", &EventHandler::path_duration)

		.def("get_button", &EventHandler::get_button, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_button_moved_down", &EventHandler::get_button_moved_down, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_button_moved_up", &EventHandler::get_button_moved_up, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_value", &EventHandler::get_value, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_direction", &EventHandler::get_direction, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

		.def("get_button_path", &EventHandler::get_button_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_value_path", &EventHandler::get_value_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("get_direction_path", &EventHandler::get_direction_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

		.def("add_button_path", &EventHandler::add_button_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)
		.def("add_value_path", &EventHandler::add_value_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)
		.def("add_direction_path", &EventHandler::add_direction_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0, py::arg("time")=0.0f)

		.def("remove_button_path", &EventHandler::get_button_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("remove_value_path", &EventHandler::get_value_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)
		.def("remove_direction_path", &EventHandler::get_direction_path, py::arg("name"), py::arg("device_type"), py::arg("device_index")=0)

		.def("update", &EventHandler::update, py::arg("time"));

	return m.ptr();
}
