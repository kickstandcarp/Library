#ifndef PARAMETRIC_CURVE_SUPPORT_HPP
#define PARAMETRIC_CURVE_SUPPORT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

template <class T>
struct CurveVertex
{
        T       value;
        float   t;
};

template <class T> bool     curve_vertex_eq(const CurveVertex<T> &a, const CurveVertex<T> &b);
template <class T> bool     curve_vertex_cmp(const CurveVertex<T> &a, const CurveVertex<T> &b);

template <class T> float	curve_vertex_distance(const CurveVertex<T> &a, const CurveVertex<T> &b);
template <> float			curve_vertex_distance(const CurveVertex<bool> &a, const CurveVertex<bool> &b);
template <> float			curve_vertex_distance(const CurveVertex<glm::quat> &a, const CurveVertex<glm::quat> &b);



template <class T>
bool curve_vertex_eq(const CurveVertex<T> &a, const CurveVertex<T> &b)
{
    return a.t == b.t;
}

template <class T>
bool curve_vertex_cmp(const CurveVertex<T> &a, const CurveVertex<T> &b)
{
    return a.t < b.t;
}

template <class T>
float curve_vertex_distance(const CurveVertex<T> &a, const CurveVertex<T> &b)
{
	return glm::distance(a.value, b.value);
}

#endif
