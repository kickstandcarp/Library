#include <sstream>

#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "parametric.hpp"
#include "parametric/curve.hpp"

namespace py = pybind11;

PYBIND11_PLUGIN(parametric)
{
    py::module::import("glm");

    py::module m("parametric");

	py::class_<Curve<bool>, std::shared_ptr<Curve<bool> > >(m, "CurveBool")
        .def("vertex", [] (Curve<bool> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t"))
        .def("vertices", [] (Curve<bool> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<bool>, Curve<bool>, std::shared_ptr<SegmentCurve<bool> > >(m, "SegmentCurveBool")
		.def("__init__", [] (SegmentCurve<bool> &instance, const py::iterable &py_vertices) { curve_vertices_from_iterable<bool>(instance, py_vertices); }, py::arg("vertices"))

		.def_property_readonly("curve_vertices", [] (SegmentCurve<bool> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
		.def("add_curve_vertex", [] (SegmentCurve<bool> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<bool>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<bool>::remove_curve_vertices_prior, py::arg("t"));

	py::class_<Curve<float>, std::shared_ptr<Curve<float> > >(m, "CurveFloat")
        .def_readwrite("min_vertex_distance", &Curve<float>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<float>::max_vertices)

		.def("vertex", [] (Curve<float> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t") )
		.def("vertices", [] (Curve<float> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<float>, Curve<float>, std::shared_ptr<SegmentCurve<float> > >(m, "SegmentCurveFloat")
        .def("__init__", [] (SegmentCurve<float> &instance, const py::iterable &py_vertices) { curve_vertices_from_iterable<float>(instance, py_vertices); }, py::arg("vertices"))

        .def_property_readonly("curve_vertices", [] (SegmentCurve<float> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
        .def("add_curve_vertex", [] (SegmentCurve<float> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<float>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<float>::remove_curve_vertices_prior, py::arg("t"));

	py::class_<Curve<glm::vec2>, std::shared_ptr<Curve<glm::vec2> > >(m, "CurveVec2")
        .def_readwrite("min_vertex_distance", &Curve<glm::vec2>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<glm::vec2>::max_vertices)

        .def("vertex", [] (Curve<glm::vec2> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t") )
        .def("vertices", [] (Curve<glm::vec2> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<glm::vec2>, Curve<glm::vec2>, std::shared_ptr<SegmentCurve<glm::vec2> > >(m, "SegmentCurveVec2")
        .def("__init__", [] (SegmentCurve<glm::vec2> &instance, const py::iterable &py_vertices) { curve_vertices_from_iterable<glm::vec2>(instance, py_vertices); }, py::arg("vertices"))

        .def_property_readonly("curve_vertices", [] (SegmentCurve<glm::vec2> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
        .def("add_curve_vertex", [] (SegmentCurve<glm::vec2> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<glm::vec2>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<glm::vec2>::remove_curve_vertices_prior, py::arg("t"));

	return m.ptr();
}
