#ifndef PARAMETRIC_SEGMENT_CURVE_HPP
#define PARAMETRIC_SEGMENT_CURVE_HPP

#include <cmath>
#include <iterator>
#include <algorithm>
#include <list>
#include <array>
#include <tuple>
#include "math/interpolation.hpp"
#include "parametric/curve.hpp"
#include "parametric/curve_support.hpp"

enum class CurveInterpolation : unsigned int { floor, ceil, nearest, linear, cubic };

template <class T>
class SegmentCurve: public Curve<T>
{
	public:
		SegmentCurve(const std::list<CurveVertex<T> > &curve_vertices, const CurveInterpolation interpolation);
        virtual ~SegmentCurve();

        virtual float                                       get_min_t() const;
        virtual float                                       get_max_t() const;
		const std::list<CurveVertex<T> >&	                get_curve_vertices() const;

		void								                add_curve_vertex(const CurveVertex<T> &curve_vertex);

		void								                remove_curve_vertices_prior(const float t);
		
		virtual CurveVertex<T>				                vertex(const float t) const;
        virtual std::list<CurveVertex<T> >	                vertices(const float t1, const float t2) const;

		CurveInterpolation									interpolation;

	private:
        float                                               normalized_t(typename std::list<CurveVertex<T> >::const_iterator curve_vertex, typename std::list<CurveVertex<T> >::const_iterator next_curve_vertex, const float t) const;
		T                                                   interpolate(typename std::list<CurveVertex<T> >::const_iterator curve_vertex, typename std::list<CurveVertex<T> >::const_iterator next_curve_vertex, const float t) const;
		std::tuple<std::array<float, 4>, std::array<T, 4> > cubic_hermite_ts_values(typename std::list<CurveVertex<T> >::const_iterator curve_vertex, typename std::list<CurveVertex<T> >::const_iterator next_curve_vertex) const;

		std::list<CurveVertex<T> >			                curve_vertices;
};



template <class T>
SegmentCurve<T>::SegmentCurve(const std::list<CurveVertex<T> > &curve_vertices, const CurveInterpolation interpolation)
:	interpolation(interpolation),
	curve_vertices(curve_vertices)
{
	this->curve_vertices.sort(&curve_vertex_cmp<T>);
}

template <class T>
SegmentCurve<T>::~SegmentCurve()
{

}

template <class T>
float SegmentCurve<T>::get_min_t() const
{
    return this->curve_vertices.front().t;
}

template <class T>
float SegmentCurve<T>::get_max_t() const
{
    return this->curve_vertices.back().t;
}

template <class T>
const std::list<CurveVertex<T> >& SegmentCurve<T>::get_curve_vertices() const
{
	return this->curve_vertices;
}

template <class T>
void SegmentCurve<T>::add_curve_vertex(const CurveVertex<T> &curve_vertex)
{
	auto insert_position = this->curve_vertices.end();
	if (!curve_vertex_cmp(this->curve_vertices.back(), curve_vertex))
		insert_position = std::lower_bound(this->curve_vertices.begin(), this->curve_vertices.end(), curve_vertex, &curve_vertex_cmp<T>);

	this->curve_vertices.insert(insert_position, curve_vertex);
}

template <class T>
void SegmentCurve<T>::remove_curve_vertices_prior(const float t)
{
	auto curve_vertex = this->curve_vertices.begin();
	while (curve_vertex != std::prev(this->curve_vertices.end()))
	{
		if (std::next(curve_vertex)->t <= t)
			curve_vertex++;
		else
			break;
	}
	
	this->curve_vertices.erase(this->curve_vertices.begin(), curve_vertex);
}

template <class T>
CurveVertex<T> SegmentCurve<T>::vertex(const float t) const
{
	CurveVertex<T> vertex{this->curve_vertices.front().value, t};

	auto end_curve_vertex = std::prev(this->curve_vertices.end());
	for (auto curve_vertex = this->curve_vertices.begin(); curve_vertex != end_curve_vertex; ++curve_vertex)
	{
		auto next_curve_vertex = std::next(curve_vertex);
		if ((curve_vertex == this->curve_vertices.begin() && t < curve_vertex->t) || (t >= curve_vertex->t && t <= next_curve_vertex->t) || (next_curve_vertex == end_curve_vertex && t > next_curve_vertex->t))
		{
            vertex.value = this->interpolate(curve_vertex, next_curve_vertex, t);
			break;
		}
	}

	return vertex;
}

template <class T>
std::list<CurveVertex<T> > SegmentCurve<T>::vertices(const float t1, const float t2) const
{
	std::list<CurveVertex<T> > vertices;
	for (auto curve_vertex = this->curve_vertices.begin(); curve_vertex != this->curve_vertices.end(); ++curve_vertex)
	{
		if (curve_vertex->t > t1 && curve_vertex->t < t2)
			vertices.push_back(*curve_vertex);
	}

	if (t1 >= this->curve_vertices.front().t && t1 <= this->curve_vertices.back().t)
        vertices.push_front(this->vertex(t1));

	if (t2 >= this->curve_vertices.front().t && t2 <= this->curve_vertices.back().t)
        vertices.push_back(this->vertex(t2));

    if (this->interpolation == CurveInterpolation::cubic)
        this->fill_vertices(vertices);

	return vertices;
}

template <class T>
float SegmentCurve<T>::normalized_t(typename std::list<CurveVertex<T> >::const_iterator curve_vertex, typename std::list<CurveVertex<T> >::const_iterator next_curve_vertex, const float t) const
{
    if (next_curve_vertex->t != curve_vertex->t)
        return (t - curve_vertex->t) / (next_curve_vertex->t - curve_vertex->t);
    else
        return 0.0f;
}

template <class T>
std::tuple<std::array<float, 4>, std::array<T, 4> > SegmentCurve<T>::cubic_hermite_ts_values(typename std::list<CurveVertex<T> >::const_iterator curve_vertex, typename std::list<CurveVertex<T> >::const_iterator next_curve_vertex) const
{
    std::tuple<std::array<float, 4>, std::array<T, 4> > values;

    std::get<0>(values)[0] = curve_vertex == this->curve_vertices.begin() ? -INFINITY : std::prev(curve_vertex)->t;
    std::get<0>(values)[1] = curve_vertex->t;
    std::get<0>(values)[2] = next_curve_vertex->t;
    std::get<0>(values)[3] = next_curve_vertex == std::prev(this->curve_vertices.end()) ? INFINITY : std::next(next_curve_vertex)->t;

    std::get<1>(values)[0] = curve_vertex == this->curve_vertices.begin() ? static_cast<T>(NAN) : std::prev(curve_vertex)->value;
    std::get<1>(values)[1] = curve_vertex->value;
    std::get<1>(values)[2] = next_curve_vertex->value;
    std::get<1>(values)[3] = next_curve_vertex == std::prev(this->curve_vertices.end()) ? static_cast<T>(NAN) : std::next(next_curve_vertex)->value;

    return values;
}

#endif
