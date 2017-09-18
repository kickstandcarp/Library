#ifndef PYTHON_GLM_HPP
#define PYTHON_GLM_HPP

#include <cmath>
#include <array>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

template <class A, class T, unsigned int D> A											py_get_vec(const T &instance, const int index);
template <class A, class T, unsigned int D> void										py_set_vec(T &instance, const int index, const A &value);

template <class A, class T, unsigned int R, unsigned int C> A							py_get_mat(const T &instance, const int index, const bool get_row=true);
template <class A, class T, unsigned int R, unsigned int C> void						py_set_mat(T &instance, const int index, const A &value, const bool set_row=true);

template <class T, unsigned int R, unsigned int C> py::object							py_get_mat(const T &instance, const py::iterable &iterable);
template <class A, class T, unsigned int R, unsigned int C> void						py_set_mat(T &instance, const py::iterable &iterable, const A &value);

template <class A, class T, unsigned int D> void										from_iterable(T &instance, const py::iterable &iterable);
template <unsigned int R, unsigned int C> std::tuple<std::array<int, 2>, unsigned int>  indices_from_iterable(const py::iterable &iterable);

template <class T, std::size_t D> std::tuple<std::array<T, D>, unsigned int>			iterable_to_array(const py::iterable &iterable);



template <class A, class T, unsigned int D>
A py_get_vec(const T &instance, const int index)
{
	int dimension = static_cast<int>(D);
	if (index >= dimension || index < -dimension)
		throw py::index_error();

	return instance[index >= 0 ? index : dimension+index];
}

template <class A, class T, unsigned int D>
void py_set_vec(T &instance, const int index, const A &value)
{
	int dimension = static_cast<int>(D);
	if (index >= dimension || index < -dimension)
		throw py::index_error();

	instance[index >= 0 ? index : dimension+index] = value;
}

template <class A, class T, unsigned int R, unsigned int C>
A py_get_mat(const T &instance, const int index, const bool get_row)
{
	int dimension = get_row ? static_cast<int>(R) : static_cast<int>(C);
	if (index >= dimension || index < -dimension)
		throw py::index_error();

	return get_row ? glm::row(instance, index >= 0 ? index : dimension+index) : glm::column(instance, index >= 0 ? index : dimension+index);
}

template <class A, class T, unsigned int R, unsigned int C>
void py_set_mat(T &instance, const int index, const A &value, const bool set_row)
{
	int dimension = set_row ? static_cast<int>(R) : static_cast<int>(C);
	if (index >= dimension || index < -dimension)
		throw py::index_error();

	instance = glm::row(instance, index >= 0 ? index : dimension+index, value);
}

template <class T, unsigned int R, unsigned int C>
py::object py_get_mat(const T &instance, const py::iterable &iterable)
{
	unsigned int num_indices;
	std::array<int, 2> indices;
	std::tie(indices, num_indices) = indices_from_iterable<R, C>(iterable);

	if (num_indices == 0)
		return py::cast(nullptr);
	else if (num_indices == 1)
		return py::cast(glm::row(instance, indices[0] >= 0 ? indices[0] : static_cast<int>(R)+indices[0]));
	else
		return py::cast(instance[indices[1] >= 0 ? indices[1] : static_cast<int>(C)+indices[1]][indices[0] >= 0 ? indices[0] : static_cast<int>(R)+indices[0]]);
}

template <class A, class T, unsigned int R, unsigned int C>
void py_set_mat(T &instance, const py::iterable &iterable, const A &value)
{
	unsigned int num_indices;
	std::array<int, 2> indices;
	std::tie(indices, num_indices) = indices_from_iterable<R, C>(iterable);

	if (num_indices == 1)
		instance = glm::row(instance, indices[0] >= 0 ? indices[0] : static_cast<int>(R)+indices[0], value);
	else
		instance[indices[1] >= 0 ? indices[1] : static_cast<int>(C)+indices[1]][indices[0] >= 0 ? indices[0] : static_cast<int>(R)+indices[0]] = value[0];
}

template <class A, class T, unsigned int D>
void from_iterable(T &instance, const py::iterable &iterable)
{
	unsigned int num_values;
	std::array<A, D> values;
	std::tie(values, num_values) = iterable_to_array<A, D>(iterable);

	if (num_values != D)
		throw py::cast_error();

	A* instance_ptr = glm::value_ptr(instance);
	for (unsigned int index = 0; index < D; index++)
		*(instance_ptr + index) = values[index];
}

template <unsigned int R, unsigned int C>
std::tuple<std::array<int, 2>, unsigned int> indices_from_iterable(const py::iterable &iterable)
{
	unsigned int num_indices;
	std::array<int, 2> indices;
	std::tie(indices, num_indices) = iterable_to_array<int, 2>(iterable);

	if (num_indices > 2)
		throw py::index_error();

	std::array<int, 2> shape{{static_cast<int>(R), static_cast<int>(C)}};
	for (unsigned int i = 0; i < num_indices; i++)
	{
		if (indices[i] >= shape[i] || indices[i]< -shape[i])
			throw py::index_error();
	}

	return std::make_tuple(indices, num_indices);
}

template <class T, std::size_t D>
std::tuple<std::array<T, D>, unsigned int> iterable_to_array(const py::iterable &iterable)
{
	std::array<T, D> values;

	unsigned int index = 0, num_values = 0;
	for (auto const &item : iterable)
	{
		if (index < D)
			values[index++] = py::cast<T>(item);
        num_values++;
	}

	for (; index < D; index++)
		values[index] = static_cast<T>(0);

	return std::make_tuple(values, num_values);
}

#endif
