#ifndef MATH_INTERPOLATION_HPP
#define MATH_INTERPOLATION_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

template <class T> T	floor_evaluate(const T& value, const T& next_value, const float t);
template <class T> T	ceil_evaluate(const T& value, const T& next_value, const float t);
template <class T> T	nearest_evaluate(const T& value, const T& next_value, const float t);

template <class T> T	linear_evaluate(const T& value, const T& next_value, const float t);
template <> glm::quat	linear_evaluate(const glm::quat& value, const glm::quat& next_value, const float t);

template <class T> T	cubic_hermite_evaluate(const std::array<T, 4> &coefficients, const float t);
std::array<float, 4>    cubic_hermite_coefficients(const float x0, const float x1, const float x2, const float x3, const float y0, const float y1, const float y2, const float y3); // From x1 to x2
float                   cubic_hermite_derivative(const float h0, const float h1, const float m0, const float m1, const bool front, const bool back);



template <class T>
T floor_evaluate(const T& value, const T& next_value, const float t)
{
	return t < 1.0f ? value : next_value;
}

template <class T>
T ceil_evaluate(const T& value, const T& next_value, const float t)
{
	return t <= 0.0f ? next_value : value;
}

template <class T>
T nearest_evaluate(const T& value, const T& next_value, const float t)
{
	return t <= 0.5f ? value : next_value;
}

template <class T>
T linear_evaluate(const T& value, const T& next_value, const float t)
{
	return glm::mix(value, next_value, t);
}

template <class T>
T cubic_hermite_evaluate(const std::array<T, 4> &coefficients, const float t)
{
	float t_1 = 1.0f - t;
	return coefficients[0]*t_1*t_1*t_1 + coefficients[1]*3.0f*t*t_1*t_1 + coefficients[2]*3.0f*t*t*t_1 + coefficients[3]*t*t*t;
}

#endif
