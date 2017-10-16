#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "parametric/segment_curve.hpp"
#include "math/interpolation.hpp"



template <>
bool SegmentCurve<bool>::interpolate(typename std::list<CurveVertex<bool> >::const_iterator curve_vertex, typename std::list<CurveVertex<bool> >::const_iterator next_curve_vertex, const float t) const
{
	switch(this->interpolation)
	{
		case CurveInterpolation::nearest:
			if (t == next_curve_vertex->t)
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::linear:
			if (t <= 0.5f*(curve_vertex->t + next_curve_vertex->t))
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::cubic:
			if (t <= 0.5f*(curve_vertex->t + next_curve_vertex->t))
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
	}

	throw std::runtime_error("unknown interpolation type");
}

template <>
float SegmentCurve<float>::interpolate(typename std::list<CurveVertex<float> >::const_iterator curve_vertex, typename std::list<CurveVertex<float> >::const_iterator next_curve_vertex, const float t) const
{
	switch(this->interpolation)
	{
		case CurveInterpolation::nearest:
			if (t == next_curve_vertex->t)
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::linear:
			return glm::mix(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
			break;
		case CurveInterpolation::cubic:
		{
			std::array<float, 4> ts;
			std::array<float, 4> values;
			std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
			std::array<float, 4> coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0], values[1], values[2], values[3]);
			return cubic_hermite_evaluate(this->normalized_t(curve_vertex, next_curve_vertex, t), coefficients);
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
		case CurveInterpolation::nearest:
			if (t == next_curve_vertex->t)
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::linear:
			return glm::mix(curve_vertex->value, next_curve_vertex->value, glm::vec2(this->normalized_t(curve_vertex, next_curve_vertex, t)));
			break;
		case CurveInterpolation::cubic:
		{
			std::array<float, 4> ts;
			std::array<glm::vec2, 4> values;
			std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
			glm::vec2 value;
			for (unsigned int i = 0; i < 2; i++)
			{
				std::array<float, 4> coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0][i], values[1][i], values[2][i], values[3][i]);
				value[i] = cubic_hermite_evaluate(this->normalized_t(curve_vertex, next_curve_vertex, t), coefficients);
			}
			return value;
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
		case CurveInterpolation::nearest:
			if (t == next_curve_vertex->t)
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::linear:
			return glm::mix(curve_vertex->value, next_curve_vertex->value, glm::vec3(this->normalized_t(curve_vertex, next_curve_vertex, t)));
			break;
		case CurveInterpolation::cubic:
		{
			std::array<float, 4> ts;
			std::array<glm::vec3, 4> values;
			std::tie(ts, values) = cubic_hermite_ts_values(curve_vertex, next_curve_vertex);
			glm::vec3 value;
			for (unsigned int i = 0; i < 3; i++)
			{
				std::array<float, 4> coefficients = cubic_hermite_coefficients(ts[0], ts[1], ts[2], ts[3], values[0][i], values[1][i], values[2][i], values[3][i]);
				value[i] = cubic_hermite_evaluate(this->normalized_t(curve_vertex, next_curve_vertex, t), coefficients);
			}
			return value;
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
		case CurveInterpolation::nearest:
			if (t == next_curve_vertex->t)
				return next_curve_vertex->value;
			else
				return curve_vertex->value;
			break;
		case CurveInterpolation::linear:
			return glm::slerp(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
			break;
		case CurveInterpolation::cubic:
			return glm::slerp(curve_vertex->value, next_curve_vertex->value, this->normalized_t(curve_vertex, next_curve_vertex, t));
			break;
	}

	throw std::runtime_error("unknown interpolation type");
}
