#include "parametric/curve_support.hpp"



template <>
float curve_vertex_distance(const CurveVertex<bool> &a, const CurveVertex<bool> &b)
{
    return 0.0f;
}

template <>
float curve_vertex_distance(const CurveVertex<glm::quat> &a, const CurveVertex<glm::quat> &b)
{
    return 0.0f;
}
