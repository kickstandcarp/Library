#ifndef MATH_INTERPOLATION_HPP
#define MATH_INTERPOLATION_HPP

#include <array>

float                   cubic_hermite_evaluate(const float x, const std::array<float, 4> &coefficients);
std::array<float, 4>    cubic_hermite_coefficients(const float x0, const float x1, const float x2, const float x3, const float y0, const float y1, const float y2, const float y3); // From x1 to x2
float                   cubic_hermite_derivative(const float h0, const float h1, const float m0, const float m1, const bool front, const bool back);

#endif
