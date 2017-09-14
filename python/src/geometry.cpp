#include <sstream>

#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "geometry/coordinate_transform.hpp"
#include "geometry/coordinate_transform_support.hpp"
#include "geometry/path.hpp"
#include "geometry/path_support.hpp"
#include "geometry/segmented_path.hpp"
#include "repr.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(geometry)
{
    py::module m("geometry");

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
		.def("transform_direction", &CoordinateTransform::transform_direction, py::arg("direction"), py::arg("to_global"));

	m.def("translation", &translation, py::arg("from"), py::arg("to"));
	m.def("rotation", &rotation, py::arg("from"), py::arg("to"));
	m.def("scaling", &scaling, py::arg("from"), py::arg("to"));

	m.def("transform_position", &transform_position, py::arg("position"), py::arg("from"), py::arg("to"));
	m.def("transform_direction", &transform_direction, py::arg("direction"), py::arg("from"), py::arg("to"));

	py::class_<PathVertex<glm::vec2> >(m, "PathVertex")
		.def(py::init<glm::vec2, float>(), py::arg("vertex"), py::arg("t"))

		.def_readwrite("vertex", &PathVertex<glm::vec2>::vertex)
		.def_readwrite("t", &PathVertex<glm::vec2>::t)
		
		.def("__repr__", [] (PathVertex<glm::vec2> &instance)
		{ 
			std::stringstream stream;
			stream << "PathVertex(vertex=" << to_python_string(instance.vertex) << ", t=" << to_python_string(instance.t) << ")";
			return stream.str();
		});

	py::class_<Path<glm::vec2>, std::shared_ptr<Path<glm::vec2> > >(m, "Path")
		.def("vertex", &Path<glm::vec2>::vertex, py::arg("t"))
		.def("vertices", &Path<glm::vec2>::vertices, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentedPath<glm::vec2>, Path<glm::vec2>, std::shared_ptr<SegmentedPath<glm::vec2> > >(m, "SegmentedPath")
		.def(py::init<std::list<PathVertex<glm::vec2> > >(), py::arg("vertices"))

		.def_property_readonly("path_vertices", &SegmentedPath<glm::vec2>::get_path_vertices)
		.def("add_path_vertex", &SegmentedPath<glm::vec2>::add_path_vertex, py::arg("path_vertex"))
		.def("remove_path_vertices_prior", &SegmentedPath<glm::vec2>::remove_path_vertices_prior, py::arg("t"));

	return m.ptr();
}
