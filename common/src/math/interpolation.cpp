#include <cmath>

#include "math/interpolation.hpp"



template <>
glm::quat linear_evaluate(const glm::quat& value, const glm::quat& next_value, const float t)
{
	return glm::slerp(value, next_value, t);
}

std::array<float, 4> cubic_hermite_coefficients(const float x0, const float x1, const float x2, const float x3, const float y0, const float y1, const float y2, const float y3)
{
    float h0 = x1 - x0, h1 = x2 - x1, h2 = x3 - x2;
    float m0 = h0 == 0.0f ? 0.0f : (y1 - y0) / h0, m1 = h1 == 0.0f ? 0.0f : (y2 - y1) / h1, m2 = h2 == 0.0f ? 0.0f : (y3 - y2) / h2;

    float d0, d1;
    if (std::isnan(m0) && std::isnan(m2))
        d0 = m1, d1 = m1;
    else if (std::isnan(m0))
        d0 = cubic_hermite_derivative(h1, h2, m1, m2, true, false), d1 = cubic_hermite_derivative(h1, h2, m1, m2, false, false);
    else if (std::isnan(m2))
        d0 = cubic_hermite_derivative(h0, h1, m0, m1, false, false), d1 = cubic_hermite_derivative(h0, h1, m0, m1, false, true);
    else
        d0 = cubic_hermite_derivative(h0, h1, m0, m1, false, false), d1 = cubic_hermite_derivative(h1, h2, m1, m2, false, false);

    return {{y1, y1 + (d0 / 3.0f)*(x2 - x1), y2 - (d1 / 3.0f)*(x2 - x1), y2}};
}

float cubic_hermite_derivative(const float h0, const float h1, const float m0, const float m1, const bool front, const bool back)
{
    float d;
    if (front)
    {
        d = ((2.0f*h0 + h1)*m0 - h0*m1) / (h0 + h1);
        if (d*m0 <= 0.0f)
            d = 0.0f;
        else if (m0*m1 <= 0.0f && std::abs(d) > 3.0f*std::abs(m0))
            d = 3.0f*m0;
    }
    else if (back)
    {
        d = ((2.0f*h1 + h0)*m1 - h1*m0) / (h1 + h0);
        if (d*m1 <= 0.0f)
            d = 0.0f;
        else if (m1*m0 <= 0.0f && std::abs(d) > 3.0f*std::abs(m1))
            d = 3.0f*m1;
    }
    else
    {
        d = 0.0f;
        if (m0*m1 > 0.0f)
            d = 3.0f*(h0 + h1) / (((h0 + 2.0f*h1) / m0) + ((2.0f*h0 + h1) / m1));
    }

    return d;
}
