#include <sstream>

#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "parametric.hpp"
#include "parametric/curve.hpp"

namespace py = pybind11;

PYBIND11_PLUGIN(parametric)
{
    py::module m("parametric");

	py::enum_<CurveInterpolation>(m, "CurveInterpolation")
		.value("floor", CurveInterpolation::floor)
		.value("ceil", CurveInterpolation::ceil)
		.value("nearest", CurveInterpolation::nearest)
		.value("linear", CurveInterpolation::linear)
		.value("cubic", CurveInterpolation::cubic);

	py::class_<Curve<bool>, std::shared_ptr<Curve<bool> > >(m, "CurveBool")
        .def_property_readonly("min_t", &Curve<bool>::get_min_t)
        .def_property_readonly("max_t", &Curve<bool>::get_max_t)

        .def("vertex", [] (Curve<bool> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t"))
        .def("vertices", [] (Curve<bool> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<bool>, Curve<bool>, std::shared_ptr<SegmentCurve<bool> > >(m, "SegmentCurveBool")
		.def("__init__", [] (SegmentCurve<bool> &instance, const py::iterable &py_vertices, const CurveInterpolation interpolation) { segment_curve_from_iterable<bool>(instance, py_vertices, interpolation); }, py::arg("vertices"), py::arg("intepolation"))

		.def_readwrite("interpolation", &SegmentCurve<bool>::interpolation)

		.def_property_readonly("curve_vertices", [] (SegmentCurve<bool> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
		.def("add_curve_vertex", [] (SegmentCurve<bool> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<bool>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<bool>::remove_curve_vertices_prior, py::arg("t"));

	py::class_<Curve<float>, std::shared_ptr<Curve<float> > >(m, "CurveFloat")
        .def_property_readonly("min_t", &Curve<float>::get_min_t)
        .def_property_readonly("max_t", &Curve<float>::get_max_t)

        .def_readwrite("min_vertex_distance", &Curve<float>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<float>::max_vertices)

		.def("vertex", [] (Curve<float> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t") )
		.def("vertices", [] (Curve<float> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<float>, Curve<float>, std::shared_ptr<SegmentCurve<float> > >(m, "SegmentCurveFloat")
        .def("__init__", [] (SegmentCurve<float> &instance, const py::iterable &py_vertices, const CurveInterpolation interpolation) { segment_curve_from_iterable<float>(instance, py_vertices, interpolation); }, py::arg("vertices"), py::arg("intepolation"))

		.def_readwrite("interpolation", &SegmentCurve<float>::interpolation)

        .def_property_readonly("curve_vertices", [] (SegmentCurve<float> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
        .def("add_curve_vertex", [] (SegmentCurve<float> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<float>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<float>::remove_curve_vertices_prior, py::arg("t"));

	py::class_<Curve<glm::vec2>, std::shared_ptr<Curve<glm::vec2> > >(m, "CurveVec2")
        .def_property_readonly("min_t", &Curve<glm::vec2>::get_min_t)
        .def_property_readonly("max_t", &Curve<glm::vec2>::get_max_t)

        .def_readwrite("min_vertex_distance", &Curve<glm::vec2>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<glm::vec2>::max_vertices)

        .def("vertex", [] (Curve<glm::vec2> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t") )
        .def("vertices", [] (Curve<glm::vec2> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<glm::vec2>, Curve<glm::vec2>, std::shared_ptr<SegmentCurve<glm::vec2> > >(m, "SegmentCurveVec2")
        .def("__init__", [] (SegmentCurve<glm::vec2> &instance, const py::iterable &py_vertices, const CurveInterpolation interpolation) { segment_curve_from_iterable<glm::vec2>(instance, py_vertices, interpolation); }, py::arg("vertices"), py::arg("intepolation"))

		.def_readwrite("interpolation", &SegmentCurve<glm::vec2>::interpolation)

        .def_property_readonly("curve_vertices", [] (SegmentCurve<glm::vec2> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
        .def("add_curve_vertex", [] (SegmentCurve<glm::vec2> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<glm::vec2>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<glm::vec2>::remove_curve_vertices_prior, py::arg("t"));

	py::class_<Curve<glm::vec3>, std::shared_ptr<Curve<glm::vec3> > >(m, "CurveVec3")
		.def_property_readonly("min_t", &Curve<glm::vec3>::get_min_t)
		.def_property_readonly("max_t", &Curve<glm::vec3>::get_max_t)

		.def_readwrite("min_vertex_distance", &Curve<glm::vec3>::min_vertex_distance)
		.def_readwrite("max_vertices", &Curve<glm::vec3>::max_vertices)

		.def("vertex", [] (Curve<glm::vec3> &instance, const float t) { return curve_vertex_to_tuple(instance.vertex(t)); }, py::arg("t") )
		.def("vertices", [] (Curve<glm::vec3> &instance, const float t1, const float t2) { return curve_vertices_to_list(instance.vertices(t1, t2)); }, py::arg("t1"), py::arg("t2"));

	py::class_<SegmentCurve<glm::vec3>, Curve<glm::vec3>, std::shared_ptr<SegmentCurve<glm::vec3> > >(m, "SegmentCurveVec3")
		.def("__init__", [] (SegmentCurve<glm::vec3> &instance, const py::iterable &py_vertices, const CurveInterpolation interpolation) { segment_curve_from_iterable<glm::vec3>(instance, py_vertices, interpolation); }, py::arg("vertices"), py::arg("intepolation"))

		.def_readwrite("interpolation", &SegmentCurve<glm::vec3>::interpolation)

		.def_property_readonly("curve_vertices", [] (SegmentCurve<glm::vec3> &instance) { return curve_vertices_to_list(instance.get_curve_vertices()); })
		.def("add_curve_vertex", [] (SegmentCurve<glm::vec3> &instance, const py::iterable &py_vertex) { return instance.add_curve_vertex(curve_vertex_from_iterable<glm::vec3>(py_vertex)); })
		.def("remove_curve_vertices_prior", &SegmentCurve<glm::vec3>::remove_curve_vertices_prior, py::arg("t"));

	return m.ptr();
}
