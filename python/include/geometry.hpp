#ifndef PYTHON_GEOMETRY_HPP
#define PYTHON_GEOMETRY_HPP

#include <pybind11/pybind11.h>
#include "geometry/path_vertex.hpp"

namespace py = pybind11;

template <class T> void     path_vertex_from_iterable(PathVertex<T> &instance, const py::iterable &iterable);



template <class T>
void path_vertex_from_iterable(PathVertex<T> &instance, const py::iterable &iterable)
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

    new (&instance) PathVertex<T>(vertex, t);
}

#endif
