#include <glm/glm.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "parametric/curve.hpp"
#include "parametric/segment_curve.hpp"

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

    py::class_<CurveVertex<bool> >(m, "CurveVertexBool")
        .def("__init__", [] (CurveVertex<bool> &instance, const bool value, const float t) { instance.value = value; instance.t = t; }, py::arg("value"), py::arg("t"))

        .def_readwrite("value", &CurveVertex<bool>::value)
        .def_readwrite("t", &CurveVertex<bool>::t);

    py::class_<Curve<bool>, std::shared_ptr<Curve<bool> > >(m, "CurveBool")
        .def_readwrite("min_vertex_distance", &Curve<bool>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<bool>::max_vertices)

        .def("vertex", &Curve<bool>::vertex)
        .def("vertices", &Curve<bool>::vertices);

    py::class_<SegmentCurve<bool>, Curve<bool>, std::shared_ptr<SegmentCurve<bool> > >(m, "SegmentCurveBool")
        .def(py::init<std::list<CurveVertex<bool> >, CurveInterpolation>(), py::arg("vertices"), py::arg("intepolation"));

    py::class_<CurveVertex<float> >(m, "CurveVertexFloat")
        .def("__init__", [] (CurveVertex<float> &instance, const float value, const float t) { instance.value = value; instance.t = t; }, py::arg("value"), py::arg("t"))

        .def_readwrite("value", &CurveVertex<float>::value)
        .def_readwrite("t", &CurveVertex<float>::t);

    py::class_<Curve<float>, std::shared_ptr<Curve<float> > >(m, "CurveFloat")
        .def_readwrite("min_vertex_distance", &Curve<float>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<float>::max_vertices)

        .def("vertex", &Curve<float>::vertex)
        .def("vertices", &Curve<float>::vertices);

    py::class_<SegmentCurve<float>, Curve<float>, std::shared_ptr<SegmentCurve<float> > >(m, "SegmentCurveFloat")
        .def(py::init<std::list<CurveVertex<float> >, CurveInterpolation>(), py::arg("vertices"), py::arg("intepolation"));

    py::class_<CurveVertex<glm::vec2> >(m, "CurveVertexVec2")
        .def("__init__", [] (CurveVertex<glm::vec2> &instance, const glm::vec2 &value, const float t) { instance.value = value; instance.t = t; }, py::arg("value"), py::arg("t"))

        .def_readwrite("value", &CurveVertex<glm::vec2>::value)
        .def_readwrite("t", &CurveVertex<glm::vec2>::t);

    py::class_<Curve<glm::vec2>, std::shared_ptr<Curve<glm::vec2> > >(m, "CurveVec2")
        .def_readwrite("min_vertex_distance", &Curve<glm::vec2>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<glm::vec2>::max_vertices)

        .def("vertex", &Curve<glm::vec2>::vertex)
        .def("vertices", &Curve<glm::vec2>::vertices);

    py::class_<SegmentCurve<glm::vec2>, Curve<glm::vec2>, std::shared_ptr<SegmentCurve<glm::vec2> > >(m, "SegmentCurveVec2")
        .def(py::init<std::list<CurveVertex<glm::vec2> >, CurveInterpolation>(), py::arg("vertices"), py::arg("intepolation"));

    py::class_<CurveVertex<glm::vec3> >(m, "CurveVertexVec3")
        .def("__init__", [] (CurveVertex<glm::vec3> &instance, const glm::vec3 &value, const float t) { instance.value = value; instance.t = t; }, py::arg("value"), py::arg("t"))

        .def_readwrite("value", &CurveVertex<glm::vec3>::value)
        .def_readwrite("t", &CurveVertex<glm::vec3>::t);

    py::class_<Curve<glm::vec3>, std::shared_ptr<Curve<glm::vec3> > >(m, "CurveVec3")
        .def_readwrite("min_vertex_distance", &Curve<glm::vec3>::min_vertex_distance)
        .def_readwrite("max_vertices", &Curve<glm::vec3>::max_vertices)

        .def("vertex", &Curve<glm::vec3>::vertex)
        .def("vertices", &Curve<glm::vec3>::vertices);

    py::class_<SegmentCurve<glm::vec3>, Curve<glm::vec3>, std::shared_ptr<SegmentCurve<glm::vec3> > >(m, "SegmentCurveVec3")
        .def(py::init<std::list<CurveVertex<glm::vec3> >, CurveInterpolation>(), py::arg("vertices"), py::arg("intepolation"));

    return m.ptr();
}
