#include <pybind11/pybind11.h>
#include <glm/glm.hpp>

#include "coordinate/coordinate_transform.hpp"
#include "coordinate/coordinate_transform_support.hpp"
#include "coordinate/camera.hpp"
#include "repr/coordinate_repr.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(coordinate)
{
    py::module m("coordinate");

    py::class_<CoordinateTransform, std::shared_ptr<CoordinateTransform> >(m, "CoordinateTransform")
        .def(py::init<glm::vec3, glm::quat, glm::vec3, glm::vec3>(), py::arg("translation")=glm::vec3(0.0f), py::arg("rotation")=glm::quat(1.0f, 0.0f, 0.0f, 0.0f), py::arg("scaling")=glm::vec3(1.0f), py::arg("origin_translation")=glm::vec3(0.0f))

		.def_property("translation", &CoordinateTransform::get_translation, &CoordinateTransform::set_translation)
		.def_property("rotation", &CoordinateTransform::get_rotation, &CoordinateTransform::set_rotation)
		.def_property("scaling", &CoordinateTransform::get_scaling, &CoordinateTransform::set_scaling)
		.def_property("origin_translation", &CoordinateTransform::get_origin_translation, &CoordinateTransform::set_origin_translation)
		.def_property("parent", &CoordinateTransform::get_parent, &CoordinateTransform::set_parent)

		.def_property_readonly("local_matrix", &CoordinateTransform::get_local_matrix)
		.def_property_readonly("global_matrix", &CoordinateTransform::get_global_matrix)
		.def_property_readonly("inverse_global_matrix", &CoordinateTransform::get_inverse_global_matrix)

		.def("translate", &CoordinateTransform::translate, py::arg("translation"))
		.def("rotate", &CoordinateTransform::rotate, py::arg("rotation"))
		.def("scale", &CoordinateTransform::scale, py::arg("scaling"))
		.def("translate_origin", &CoordinateTransform::translate_origin, py::arg("translation"))

		.def("transform_position", &CoordinateTransform::transform_position, py::arg("position"), py::arg("to_global"))
		.def("transform_direction", &CoordinateTransform::transform_direction, py::arg("direction"), py::arg("to_global"))

		.def("__repr__", &coordinate_transform_repr);

	m.def("translation", &translation, py::arg("from"), py::arg("to"));
	m.def("rotation", &rotation, py::arg("from"), py::arg("to"));
	m.def("scaling", &scaling, py::arg("from"), py::arg("to"));

	m.def("transform_position", &transform_position, py::arg("position"), py::arg("from"), py::arg("to"));
	m.def("transform_direction", &transform_direction, py::arg("direction"), py::arg("from"), py::arg("to"));

    py::class_<Camera>(m, "Camera")
        .def(py::init<float, float, float, float, bool>(), py::arg("width"), py::arg("height"), py::arg("near_z"), py::arg("far_z"), py::arg("orthographic"))

		.def_property_readonly("coordinate_transform", &Camera::get_coordinate_transform)
        .def_property_readonly("transform_matrix", &Camera::get_transform_matrix)

        .def("set_perspective", &Camera::set_perspective, py::arg("width"), py::arg("height"), py::arg("near_z"), py::arg("far_z"), py::arg("orthographic"))

		.def("__repr__", &camera_repr);

	return m.ptr();
}
