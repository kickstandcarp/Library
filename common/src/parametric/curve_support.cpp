#include <stdexcept>

#include "parametric/curve_support.hpp"



template <>
float curve_vertex_distance<bool>(const CurveVertex<bool> &a, const CurveVertex<bool> &b)
{
	throw std::runtime_error("not implemented yet");
}

template <>
float curve_vertex_distance<glm::quat>(const CurveVertex<glm::quat> &a, const CurveVertex<glm::quat> &b)
{
	throw std::runtime_error("not implemented yet");
}
