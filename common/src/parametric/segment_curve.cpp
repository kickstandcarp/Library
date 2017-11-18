#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "parametric/segment_curve.hpp"



template <>
bool SegmentCurve<bool>::interpolate(typename std::list<CurveVertex<bool> >::const_iterator curve_vertex, typename std::list<CurveVertex<bool> >::const_iterator next_curve_vertex, const float t) const
{
    switch(this->interpolation)
    {
        case CurveInterpolation::floor:
            return floor_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::ceil:
            return ceil_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::nearest:
            return nearest_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::linear:
            throw std::runtime_error("invalid interpolation for curve type");
            break;
        case CurveInterpolation::cubic:
            throw std::runtime_error("invalid interpolation for curve type");
            break;
    }

    throw std::runtime_error("unknown interpolation type");
}

template <>
float SegmentCurve<float>::interpolate(typename std::list<CurveVertex<float> >::const_iterator curve_vertex, typename std::list<CurveVertex<float> >::const_iterator next_curve_vertex, const float t) const
{
    switch(this->interpolation)
    {
        case CurveInterpolation::floor:
            return floor_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::ceil:
            return ceil_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::nearest:
            return nearest_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::linear:
            return linear_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::cubic:
        {
            std::array<float, 4> ts;
            std::array<float, 4> values;
            std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
            std::array<float, 4> coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0], values[1], values[2], values[3]);
            return cubic_hermite_evaluate(coefficients, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        }
    }

    throw std::runtime_error("unknown interpolation type");
}

template <>
glm::vec2 SegmentCurve<glm::vec2>::interpolate(typename std::list<CurveVertex<glm::vec2> >::const_iterator curve_vertex, typename std::list<CurveVertex<glm::vec2> >::const_iterator next_curve_vertex, const float t) const
{
    switch(this->interpolation)
    {
        case CurveInterpolation::floor:
            return floor_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::ceil:
            return ceil_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::nearest:
            return nearest_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::linear:
            return linear_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::cubic:
        {
            std::array<float, 4> ts;
            std::array<glm::vec2, 4> values;
            std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
            std::array<float, 4> x_coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0].x, values[1].x, values[2].x, values[3].x);
            std::array<float, 4> y_coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0].y, values[1].y, values[2].y, values[3].y);
            return cubic_hermite_evaluate(std::array<glm::vec2, 4>{{glm::vec2(x_coefficients[0], y_coefficients[0]), glm::vec2(x_coefficients[1], y_coefficients[1]), glm::vec2(x_coefficients[2], y_coefficients[2]), glm::vec2(x_coefficients[3], y_coefficients[3])}}, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        }
    }

    throw std::runtime_error("unknown interpolation type");
}

template <>
glm::vec3 SegmentCurve<glm::vec3>::interpolate(typename std::list<CurveVertex<glm::vec3> >::const_iterator curve_vertex, typename std::list<CurveVertex<glm::vec3> >::const_iterator next_curve_vertex, const float t) const
{
    switch(this->interpolation)
    {
        case CurveInterpolation::floor:
            return floor_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::ceil:
            return ceil_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::nearest:
            return nearest_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::linear:
            return linear_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::cubic:
        {
            std::array<float, 4> ts;
            std::array<glm::vec3, 4> values;
            std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
            std::array<float, 4> x_coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0].x, values[1].x, values[2].x, values[3].x);
            std::array<float, 4> y_coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0].y, values[1].y, values[2].y, values[3].y);
            std::array<float, 4> z_coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0].z, values[1].z, values[2].z, values[3].z);
            return cubic_hermite_evaluate(std::array<glm::vec3, 4>{{glm::vec3(x_coefficients[0], y_coefficients[0], z_coefficients[0]), glm::vec3(x_coefficients[1], y_coefficients[1], z_coefficients[1]), glm::vec3(x_coefficients[2], y_coefficients[2], z_coefficients[2]), glm::vec3(x_coefficients[3], y_coefficients[3], z_coefficients[3])}}, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        }
    }

    throw std::runtime_error("unknown interpolation type");
}

template <>
glm::quat SegmentCurve<glm::quat>::interpolate(typename std::list<CurveVertex<glm::quat> >::const_iterator curve_vertex, typename std::list<CurveVertex<glm::quat> >::const_iterator next_curve_vertex, const float t) const
{
    switch(this->interpolation)
    {
        case CurveInterpolation::floor:
            return floor_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::ceil:
            return ceil_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::nearest:
            return nearest_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::linear:
            return linear_evaluate(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
            break;
        case CurveInterpolation::cubic:
            throw std::runtime_error("invalid interpolation for curve type");
            break;
    }

    throw std::runtime_error("unknown interpolation type");
}
