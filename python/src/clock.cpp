#include <pybind11/pybind11.h>

#include "clock.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(clock)
{
    py::module m("clock");

    py::class_<Clock>(m, "Clock")
		.def(py::init<>())

        .def_readwrite("time", &Clock::time)
        .def_readwrite("elapsed_time", &Clock::elapsed_time)
        .def_readwrite("step_count", &Clock::step_count)

        .def("step", &Clock::step, py::arg("elapsed_time"));

	return m.ptr();
}
