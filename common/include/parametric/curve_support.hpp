#ifndef PARAMETRIC_CURVE_SUPPORT_HPP
#define PARAMETRIC_CURVE_SUPPORT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

template <class T>
struct CurveVertex
{
		T		value;
		float   t;
};

template <class T> bool	    curve_vertex_eq(const CurveVertex<T> &a, const CurveVertex<T> &b);
template <class T> bool     curve_vertex_cmp(const CurveVertex<T> &a, const CurveVertex<T> &b);



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

#endif
