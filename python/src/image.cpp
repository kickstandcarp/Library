#include <pybind11/pybind11.h>

#include "image/gaussian_filter.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(image)
{
    py::module m("image");

    py::class_<GaussianFilter>(m, "GaussianFilter")
		.def(py::init<Window>(), py::arg("window"))

        .def_readwrite("size", &GaussianFilter::size)
        .def_readwrite("width", &GaussianFilter::width)
        .def_readwrite("amplitude", &GaussianFilter::amplitude)

        .def_readwrite("blend", &GaussianFilter::blend)
        .def_readwrite("source_blend_factor", &GaussianFilter::source_blend_factor)
        .def_readwrite("destination_blend_factor", &GaussianFilter::destination_blend_factor)

        .def_readwrite("source_frame_buffer_name", &GaussianFilter::source_frame_buffer_name)
        .def_readwrite("source_color_attachment_index", &GaussianFilter::source_color_attachment_index)
        .def_readwrite("intermediate_frame_buffer_name", &GaussianFilter::intermediate_frame_buffer_name)
        .def_readwrite("intermediate_color_attachment_index", &GaussianFilter::intermediate_color_attachment_index)
        .def_readwrite("destination_frame_buffer_name", &GaussianFilter::destination_frame_buffer_name)
        .def_readwrite("destination_color_attachment_index", &GaussianFilter::destination_color_attachment_index)

        .def("apply", &GaussianFilter::apply, py::arg("window"));

	return m.ptr();
}
