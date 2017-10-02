#ifndef PYTHON_PARAMETRIC_HPP
#define PYTHON_PARAMETRIC_HPP

#include <list>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "parametric/curve_support.hpp"
#include "parametric/segment_curve.hpp"

namespace py = pybind11;

template <class T> void             curve_vertices_from_iterable(SegmentCurve<T> &instance, const py::iterable &iterable);
template <class T> CurveVertex<T>   curve_vertex_from_iterable(const py::iterable &iterable);

template <class T> py::list         curve_vertices_to_list(const std::list<CurveVertex<T> > &curve_vertex_list);
template <class T> py::tuple        curve_vertex_to_tuple(const CurveVertex<T> &curve_vertex);



template <class T>
void curve_vertices_from_iterable(SegmentCurve<T> &instance, const py::iterable &iterable)
{
    std::list<CurveVertex<T> > vertices;

    for (auto const &item : iterable)
        vertices.push_back(curve_vertex_from_iterable<T>(py::cast<py::iterable>(item)));

    new (&instance) SegmentCurve<T>(vertices);
}

template <class T>
CurveVertex<T> curve_vertex_from_iterable(const py::iterable &iterable)
{
    T vertex;
    float t;

    unsigned int index = 0;
    for (auto const &item : iterable)
    {
        if (index == 0)
            vertex = py::cast<T>(item);
        else if (index == 1)
            t = py::cast<float>(item);
        index++;
    }

    if (index > 2)
        throw py::cast_error();

    return {vertex, t};
}

template <class T>
py::list curve_vertices_to_list(const std::list<CurveVertex<T> > &curve_vertex_list)
{
    py::list py_list;
    for (auto const &curve_vertex : curve_vertex_list)
        py_list.append(curve_vertex_to_tuple(curve_vertex));

    return py_list;
}

template <class T>
py::tuple curve_vertex_to_tuple(const CurveVertex<T> &curve_vertex)
{
    return py::make_tuple(curve_vertex.value, curve_vertex.t);
}

#endif
