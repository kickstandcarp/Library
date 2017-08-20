#include <pybind11/operators.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "module/glm.hpp"
#include "repr/glm_repr.hpp"

namespace py = pybind11;



PYBIND11_PLUGIN(glm)
{
    py::module m("glm");

	py::class_<glm::ivec2>(m, "ivec2")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<int, int>())
		.def(py::init<glm::ivec2>())
		.def("__init__", [] (glm::ivec2 &instance, const py::iterable &iterable) { from_iterable<int, glm::ivec2, 2>(instance, iterable); })

		.def_readwrite("x", &glm::ivec2::x)
		.def_readwrite("y", &glm::ivec2::y)
		.def_readwrite("r", &glm::ivec2::r)
		.def_readwrite("g", &glm::ivec2::g)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(int() + py::self)
		.def(py::self + int())
		.def(py::self += int())
		.def(int() - py::self)
		.def(py::self - int())
		.def(py::self -= int())
		.def(int() * py::self)
		.def(py::self * int())
		.def(py::self *= int())
		.def(int() / py::self)
		.def(py::self / int())
		.def(py::self /= int())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def("__len__", [] (const glm::ivec2 &instance) { return 2; })
		.def("__getitem__", [] (const glm::ivec2 &instance, const long index) { return py_get_vec<int, glm::ivec2, 2>(instance, index); })
		.def("__setitem__", [] (glm::ivec2 &instance, const long index, const int value) { return py_set_vec<int, glm::ivec2, 2>(instance, index, value); })
		.def("__iter__", [] (const glm::ivec2 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+2); }, py::keep_alive<0, 1>())
		.def("__repr__", &ivec2_repr);

	py::implicitly_convertible<py::tuple, glm::ivec2>();
	py::implicitly_convertible<py::list, glm::ivec2>();

	py::class_<glm::ivec3>(m, "ivec3")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<int, int, int>())
		.def(py::init<glm::ivec3>())
		.def("__init__", [] (glm::ivec3 &instance, const py::iterable &iterable) { from_iterable<int, glm::ivec3, 3>(instance, iterable); })

		.def_readwrite("x", &glm::ivec3::x)
		.def_readwrite("y", &glm::ivec3::y)
		.def_readwrite("z", &glm::ivec3::z)
		.def_readwrite("r", &glm::ivec3::r)
		.def_readwrite("g", &glm::ivec3::g)
		.def_readwrite("b", &glm::ivec3::b)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(int() + py::self)
		.def(py::self + int())
		.def(py::self += int())
		.def(int() - py::self)
		.def(py::self - int())
		.def(py::self -= int())
		.def(int() * py::self)
		.def(py::self * int())
		.def(py::self *= int())
		.def(int() / py::self)
		.def(py::self / int())
		.def(py::self /= int())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def("__len__", [] (const glm::ivec3 &instance) { return 3; })
		.def("__getitem__", [] (const glm::ivec3 &instance, const long index) { return py_get_vec<int, glm::ivec3, 3>(instance, index); })
		.def("__setitem__", [] (glm::ivec3 &instance, const long index, const int value) { return py_set_vec<int, glm::ivec3, 3>(instance, index, value); })
		.def("__iter__", [] (const glm::ivec2 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+3); }, py::keep_alive<0, 1>())
		.def("__repr__", &ivec3_repr);

	py::implicitly_convertible<py::tuple, glm::ivec3>();
	py::implicitly_convertible<py::list, glm::ivec3>();

	py::class_<glm::ivec4>(m, "ivec4")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<int, int, int, int>())
		.def(py::init<glm::ivec4>())
		.def("__init__", [] (glm::ivec4 &instance, const py::iterable &iterable) { from_iterable<int, glm::ivec4, 4>(instance, iterable); })

		.def_readwrite("x", &glm::ivec4::x)
		.def_readwrite("y", &glm::ivec4::y)
		.def_readwrite("z", &glm::ivec4::z)
		.def_readwrite("w", &glm::ivec4::w)
		.def_readwrite("r", &glm::ivec4::r)
		.def_readwrite("g", &glm::ivec4::g)
		.def_readwrite("b", &glm::ivec4::b)
		.def_readwrite("a", &glm::ivec4::a)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(int() + py::self)
		.def(py::self + int())
		.def(py::self += int())
		.def(int() - py::self)
		.def(py::self - int())
		.def(py::self -= int())
		.def(int() * py::self)
		.def(py::self * int())
		.def(py::self *= int())
		.def(int() / py::self)
		.def(py::self / int())
		.def(py::self /= int())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def("__len__", [] (const glm::ivec4 &instance) { return 4; })
		.def("__getitem__", [] (const glm::ivec4 &instance, const long index) { return py_get_vec<int, glm::ivec4, 4>(instance, index); })
		.def("__setitem__", [] (glm::ivec4 &instance, const long index, const int value) { return py_set_vec<int, glm::ivec4, 4>(instance, index, value); })
		.def("__iter__", [] (const glm::ivec2 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+4); }, py::keep_alive<0, 1>())
		.def("__repr__", &ivec4_repr);

	py::implicitly_convertible<py::tuple, glm::ivec4>();
	py::implicitly_convertible<py::list, glm::ivec4>();

    py::class_<glm::vec2>(m, "vec2")
	    .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float>())
		.def(py::init<glm::vec2>())
		.def("__init__", [] (glm::vec2 &instance, const py::iterable &iterable) { from_iterable<float, glm::vec2, 2>(instance, iterable); })

        .def_readwrite("x", &glm::vec2::x)
        .def_readwrite("y", &glm::vec2::y)
		.def_readwrite("r", &glm::vec2::r)
		.def_readwrite("g", &glm::vec2::g)

        .def(-py::self)
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * py::self)
        .def(py::self *= py::self)
        .def(py::self / py::self)
        .def(py::self /= py::self)

        .def(float() + py::self)
        .def(py::self + float())
        .def(py::self += float())
        .def(float() - py::self)
        .def(py::self - float())
        .def(py::self -= float())
        .def(float() * py::self)
        .def(py::self * float())
        .def(py::self *= float())
        .def(float() / py::self)
        .def(py::self / float())
        .def(py::self /= float())
    
        .def(py::self == py::self)
        .def(py::self != py::self)

        .def("__len__", [] (const glm::vec2 &instance) { return 2; })
        .def("__getitem__", [] (const glm::vec2 &instance, const long index) { return py_get_vec<float, glm::vec2, 2>(instance, index); })
        .def("__setitem__", [] (glm::vec2 &instance, const long index, const float value) { return py_set_vec<float, glm::vec2, 2>(instance, index, value); })
		.def("__iter__", [] (const glm::vec2 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+2); }, py::keep_alive<0, 1>())
		.def("__repr__", &vec2_repr);
	
	py::implicitly_convertible<py::tuple, glm::vec2>();
    py::implicitly_convertible<py::list, glm::vec2>();

	m.def("dot", (float (*)(const glm::vec2 &, const glm::vec2 &)) &glm::dot);
	m.def("length", (float (*)(const glm::vec2 &)) &glm::length);
    m.def("normalize", (glm::vec2 (*)(const glm::vec2 &)) &glm::normalize);
	m.def("distance", (float (*)(const glm::vec2 &, const glm::vec2 &)) &glm::distance);

    py::class_<glm::vec3>(m, "vec3")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float, float>())
		.def(py::init<glm::vec3>())
		.def("__init__", [] (glm::vec3 &instance, const py::iterable &iterable) { from_iterable<float, glm::vec3, 3>(instance, iterable); })

        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z)
		.def_readwrite("r", &glm::vec3::r)
		.def_readwrite("g", &glm::vec3::g)
		.def_readwrite("b", &glm::vec3::b)

        .def(-py::self)
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * py::self)
        .def(py::self *= py::self)
        .def(py::self / py::self)
        .def(py::self /= py::self)

        .def(float() + py::self)
        .def(py::self + float())
        .def(py::self += float())
        .def(float() - py::self)
        .def(py::self - float())
        .def(py::self -= float())
        .def(float() * py::self)
        .def(py::self * float())
        .def(py::self *= float())
        .def(float() / py::self)
        .def(py::self / float())
        .def(py::self /= float())

        .def(py::self == py::self)
        .def(py::self != py::self)

        .def("__len__", [] (const glm::vec3 &instance) { return 3; })
		.def("__getitem__", [] (const glm::vec3 &instance, const long index) { return py_get_vec<float, glm::vec3, 3>(instance, index); })
		.def("__setitem__", [] (glm::vec3 &instance, const long index, const float value) { return py_set_vec<float, glm::vec3, 3>(instance, index, value); })
		.def("__iter__", [] (const glm::vec3 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+3); }, py::keep_alive<0, 1>())
        .def("__repr__", &vec3_repr);

    py::implicitly_convertible<py::list, glm::vec3>();
    py::implicitly_convertible<py::tuple, glm::vec3>();

	m.def("dot", (float (*)(const glm::vec3 &, const glm::vec3 &)) &glm::dot);
	m.def("cross", (glm::vec3 (*)(const glm::vec3 &, const glm::vec3 &)) &glm::cross);
	m.def("length", (float (*)(const glm::vec3 &)) &glm::length);
	m.def("normalize", (glm::vec3 (*)(const glm::vec3 &)) &glm::normalize);
	m.def("distance", (float (*)(const glm::vec3 &, const glm::vec3 &)) &glm::distance);

	py::class_<glm::vec4>(m, "vec4")
		.def(py::init<>())
		.def(py::init<float>())
		.def(py::init<float, float, float, float>())
		.def(py::init<glm::vec4>())
		.def("__init__", [] (glm::vec4 &instance, const py::iterable &iterable) { from_iterable<float, glm::vec4, 4>(instance, iterable); })

		.def_readwrite("x", &glm::vec4::x)
		.def_readwrite("y", &glm::vec4::y)
		.def_readwrite("z", &glm::vec4::z)
		.def_readwrite("w", &glm::vec4::w)
		.def_readwrite("r", &glm::vec4::r)
		.def_readwrite("g", &glm::vec4::g)
		.def_readwrite("b", &glm::vec4::b)
		.def_readwrite("a", &glm::vec4::a)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(float() + py::self)
		.def(py::self + float())
		.def(py::self += float())
		.def(float() - py::self)
		.def(py::self - float())
		.def(py::self -= float())
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self *= float())
		.def(float() / py::self)
		.def(py::self / float())
		.def(py::self /= float())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def("__len__", [] (const glm::vec4 &instance) { return 4; })
		.def("__getitem__", [] (const glm::vec4 &instance, const long index) { return py_get_vec<float, glm::vec4, 4>(instance, index); })
		.def("__setitem__", [] (glm::vec4 &instance, const long index, const float value) { return py_set_vec<float, glm::vec4, 4>(instance, index, value); })
		.def("__iter__", [] (const glm::vec4 &instance) { return py::make_iterator(glm::value_ptr(instance), glm::value_ptr(instance)+4); }, py::keep_alive<0, 1>())
		.def("__repr__", &vec4_repr);

	py::implicitly_convertible<py::list, glm::vec4>();
	py::implicitly_convertible<py::tuple, glm::vec4>();

	m.def("dot", (float (*)(const glm::vec4 &, const glm::vec4 &)) &glm::dot);
	m.def("length", (float (*)(const glm::vec4 &)) &glm::length);
	m.def("normalize", (glm::vec4 (*)(const glm::vec4 &)) &glm::normalize);
	m.def("distance", (float (*)(const glm::vec4 &, const glm::vec4 &)) &glm::distance);

	py::class_<glm::quat>(m, "quat")
		.def(py::init<>())
		.def(py::init<float, float, float, float>(), py::arg("w"), py::arg("x"), py::arg("y"), py::arg("z"))
		.def(py::init<glm::vec3>(), py::arg("angles"))
		.def("__init__", [] (glm::quat &instance, const float angle_x, const float angle_y, const float angle_z) { new (&instance) glm::quat(glm::vec3(angle_x, angle_y, angle_z)); }, py::arg("angle_x"), py::arg("angle_y"), py::arg("angle_z"))
		.def("__init__", [] (glm::quat &instance, const glm::vec3 &axis, const float angle) { new (&instance) glm::quat(glm::angleAxis(angle, axis)); }, py::arg("axis"), py::arg("angle"))

		.def_readwrite("x", &glm::quat::x)
		.def_readwrite("y", &glm::quat::y)
		.def_readwrite("z", &glm::quat::z)
		.def_readwrite("w", &glm::quat::w)

		.def(py::self * py::self)
		.def(py::self *= py::self)

		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self *= float())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def_property_readonly("conj", [] (const glm::quat &instance) { return glm::conjugate(instance); })
		.def_property("axis", [] (const glm::quat &instance) { return glm::axis(instance); }, [] (glm::quat &instance, glm::vec3 &axis) { instance = glm::angleAxis(glm::angle(instance), axis); })
		.def_property("angle", [] (const glm::quat &instance) { return glm::angle(instance); }, [] (glm::quat &instance, float angle) { instance = glm::angleAxis(angle, glm::axis(instance)); })
		.def_property("angles", [] (const glm::quat &instance) { return glm::eulerAngles(instance); }, [] (glm::quat &instance, glm::vec3 &angles) { instance = glm::quat(angles); })

		.def("__repr__", &quat_repr);

	m.def("exp", (glm::quat (*)(const glm::quat &)) &glm::exp);
	m.def("normalize", (glm::quat (*)(const glm::quat &)) &glm::normalize);
	m.def("length", (float (*)(const glm::quat &)) &glm::length);
	m.def("rotate", [] (const glm::vec3 &vector, const glm::quat &quat) { return glm::rotate(quat, vector); }, py::arg("vector"), py::arg("quaternion"));
	m.def("rotation", [] (const glm::vec3 &vector_1, const glm::vec3 &vector_2) { return glm::rotation(glm::normalize(vector_1), glm::normalize(vector_2)); }, py::arg("vector_1"), py::arg("vector_2"));

	py::class_<glm::mat2x2>(m, "mat2x2")
		.def(py::init<>())
		.def(py::init<float>())
		.def("__init__", [] (glm::mat2x2 &instance, const float m00, const float m01, const float m10, const float m11, const bool row_major) { new (&instance) glm::mat2x2(m00, m01, m10, m11); instance = row_major ? glm::transpose(instance) : instance; }, py::arg("m00"), py::arg("m01"), py::arg("m10"), py::arg("m11"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat2x2 &instance, const glm::vec2 &r0, const glm::vec2 &r1, const bool row_major) { new (&instance) glm::mat2x2(r0, r1);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("r0"), py::arg("r1"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat2x2 &instance, const py::iterable &iterable, const bool row_major) { from_iterable<float, glm::mat2x2, 4>(instance, iterable);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("iterable"), py::arg("row_major")=true)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(glm::vec2() * py::self)
		.def(py::self * glm::vec2())
		.def(py::self *= glm::vec2())

		.def(float() + py::self)
		.def(py::self + float())
		.def(py::self += float())
		.def(float() - py::self)
		.def(py::self - float())
		.def(py::self -= float())
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self *= float())
		.def(float() / py::self)
		.def(py::self / float())
		.def(py::self /= float())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def_property_readonly("T", [] (const glm::mat2x2 &instance) { return glm::transpose(instance); })
		.def("row", [] (const glm::mat2x2 &instance, const long row) { return glm::row(instance, row); }, py::arg("row"))
		.def("column", [] (const glm::mat2x2 &instance, const long column) { return glm::column(instance, column); }, py::arg("column"))

		.def("__getitem__", [] (const glm::mat2x2 &instance, const long row) { return py_get_mat<glm::vec2, glm::mat2x2, 2, 2>(instance, row, true); })
		.def("__getitem__", [] (const glm::mat2x2 &instance, const py::iterable &iterable) { return py_get_mat<glm::mat2x2, 2, 2>(instance, iterable); })
		.def("__setitem__", [] (glm::mat2x2 &instance, const long row, const float value) { py_set_mat<glm::vec2, glm::mat2x2, 2, 2>(instance, row, glm::vec2(value), true); })
		.def("__setitem__", [] (glm::mat2x2 &instance, const long row, const glm::vec2 &value) { py_set_mat<glm::vec2, glm::mat2x2, 2, 2>(instance, row, value, true); })
		.def("__setitem__", [] (glm::mat2x2 &instance, const py::iterable &iterable, const float value) { py_set_mat<glm::vec2, glm::mat2x2, 2, 2>(instance, iterable, glm::vec2(value)); })
		.def("__setitem__", [] (glm::mat2x2 &instance, py::iterable iterable, glm::vec2 &value) { py_set_mat<glm::vec2, glm::mat2x2, 2, 2>(instance, iterable, value); })
		.def("__repr__", &mat2x2_repr);

	m.def("inverse", (glm::mat2x2 (*)(const glm::mat2x2 &)) &glm::inverse);

	py::class_<glm::mat3x3>(m, "mat3x3")
		.def(py::init<>())
		.def(py::init<float>())
		.def("__init__", [] (glm::mat3x3 &instance, const float m00, const float m01, const float m02, const float m10, const float m11, const float m12, const float m20, const float m21, const float m22, const bool row_major) { new (&instance) glm::mat3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22); instance = row_major ? glm::transpose(instance) : instance; }, py::arg("m00"), py::arg("m01"), py::arg("m02"), py::arg("m10"), py::arg("m11"), py::arg("m12"), py::arg("m20"), py::arg("m21"), py::arg("m22"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat3x3 &instance, const glm::vec3 &r0, const glm::vec3 &r1, const glm::vec3 &r2, const bool row_major) { new (&instance) glm::mat3x3(r0, r1, r2);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("r0"), py::arg("r1"), py::arg("r2"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat3x3 &instance, const py::iterable &iterable, const bool row_major) { from_iterable<float, glm::mat3x3, 9>(instance, iterable);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("iterable"), py::arg("row_major")=true)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(glm::vec3() * py::self)
		.def(py::self * glm::vec3())
		.def(py::self *= glm::vec3())

		.def(float() + py::self)
		.def(py::self + float())
		.def(py::self += float())
		.def(float() - py::self)
		.def(py::self - float())
		.def(py::self -= float())
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self *= float())
		.def(float() / py::self)
		.def(py::self / float())
		.def(py::self /= float())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def_property_readonly("T", [] (const glm::mat3x3 &instance) { return glm::transpose(instance); })
		.def("row", [] (const glm::mat3x3 &instance, const long row) { return glm::row(instance, row); }, py::arg("row"))
		.def("column", [] (const glm::mat3x3 &instance, const long column) { return glm::column(instance, column); }, py::arg("column"))

		.def("__getitem__", [] (const glm::mat3x3 &instance, const long row) { return py_get_mat<glm::vec3, glm::mat3x3, 3, 3>(instance, row, true); })
		.def("__getitem__", [] (const glm::mat3x3 &instance, const py::iterable &iterable) { return py_get_mat<glm::mat3x3, 3, 3>(instance, iterable); })
		.def("__setitem__", [] (glm::mat3x3 &instance, const long row, const float value) { py_set_mat<glm::vec3, glm::mat3x3, 3, 3>(instance, row, glm::vec3(value), true); })
		.def("__setitem__", [] (glm::mat3x3 &instance, const long row, const glm::vec3 &value) { py_set_mat<glm::vec3, glm::mat3x3, 3, 3>(instance, row, value, true); })
		.def("__setitem__", [] (glm::mat3x3 &instance, const py::iterable &iterable, const float value) { py_set_mat<glm::vec3, glm::mat3x3, 3, 3>(instance, iterable, glm::vec3(value)); })
		.def("__setitem__", [] (glm::mat3x3 &instance, py::iterable iterable, glm::vec3 &value) { py_set_mat<glm::vec3, glm::mat3x3, 3, 3>(instance, iterable, value); })
		.def("__repr__", &mat3x3_repr);

	m.def("inverse", (glm::mat3x3 (*)(const glm::mat3x3 &)) &glm::inverse);

	py::class_<glm::mat4x4>(m, "mat4x4")
		.def(py::init<>())
		.def(py::init<float>())
		.def("__init__", [] (glm::mat4x4 &instance, const float m00, const float m01, const float m02, const float m03, const float m10, const float m11, const float m12, const float m13, const float m20, const float m21, const float m22, const float m23, const float m30, const float m31, const float m32, const float m33, const bool row_major) { new (&instance) glm::mat4x4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33); instance = row_major ? glm::transpose(instance) : instance; }, py::arg("m00"), py::arg("m01"), py::arg("m02"), py::arg("m03"), py::arg("m10"), py::arg("m11"), py::arg("m12"), py::arg("m13"), py::arg("m20"), py::arg("m21"), py::arg("m22"), py::arg("m23"), py::arg("m30"), py::arg("m31"), py::arg("m32"), py::arg("m33"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat4x4 &instance, const glm::vec4 &r0, const glm::vec4 &r1, const glm::vec4 &r2, const glm::vec4 &r3, const bool row_major) { new (&instance) glm::mat4x4(r0, r1, r2, r3);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("r0"), py::arg("r1"), py::arg("r2"), py::arg("r3"), py::arg("row_major")=true)
		.def("__init__", [] (glm::mat4x4 &instance, const py::iterable &iterable, const bool row_major) { from_iterable<float, glm::mat4x4, 16>(instance, iterable);  instance = row_major ? glm::transpose(instance) : instance; }, py::arg("iterable"), py::arg("row_major")=true)

		.def(-py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self / py::self)
		.def(py::self /= py::self)

		.def(glm::vec4() * py::self)
		.def(py::self * glm::vec4())
		.def(py::self *= glm::vec4())

		.def(float() + py::self)
		.def(py::self + float())
		.def(py::self += float())
		.def(float() - py::self)
		.def(py::self - float())
		.def(py::self -= float())
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self *= float())
		.def(float() / py::self)
		.def(py::self / float())
		.def(py::self /= float())

		.def(py::self == py::self)
		.def(py::self != py::self)

		.def_property_readonly("T", [] (const glm::mat4x4 &instance) { return glm::transpose(instance); })
		.def("row", [] (const glm::mat4x4 &instance, const long row) { return glm::row(instance, row); }, py::arg("row"))
		.def("column", [] (const glm::mat4x4 &instance, const long column) { return glm::column(instance, column); }, py::arg("column"))

		.def("__getitem__", [] (const glm::mat4x4 &instance, const long row) { return py_get_mat<glm::vec4, glm::mat4x4, 4, 4>(instance, row, true); })
		.def("__getitem__", [] (const glm::mat4x4 &instance, const py::iterable &iterable) { return py_get_mat<glm::mat4x4, 4, 4>(instance, iterable); })
		.def("__setitem__", [] (glm::mat4x4 &instance, const long row, const float value) { py_set_mat<glm::vec4, glm::mat4x4, 4, 4>(instance, row, glm::vec4(value), true); })
		.def("__setitem__", [] (glm::mat4x4 &instance, const long row, const glm::vec4 &value) { py_set_mat<glm::vec4, glm::mat4x4, 4, 4>(instance, row, value, true); })
		.def("__setitem__", [] (glm::mat4x4 &instance, const py::iterable &iterable, const float value) { py_set_mat<glm::vec4, glm::mat4x4, 4, 4>(instance, iterable, glm::vec4(value)); })
		.def("__setitem__", [] (glm::mat4x4 &instance, py::iterable iterable, glm::vec4 &value) { py_set_mat<glm::vec4, glm::mat4x4, 4, 4>(instance, iterable, value); })
		.def("__repr__", &mat4x4_repr);

	m.def("inverse", (glm::mat4x4 (*)(const glm::mat4x4 &)) &glm::inverse);

	return m.ptr();
}
