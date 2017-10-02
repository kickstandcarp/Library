#ifndef PARAMETRIC_CURVE_HPP
#define PARAMETRIC_CURVE_HPP

#include <stdexcept>
#include <cmath>
#include <list>
#include "parametric/curve_support.hpp"

template <class T>
class Curve
{
	public:
		Curve(const float min_vertex_distance=INFINITY);
        virtual ~Curve();
		
		virtual CurveVertex<T>				vertex(const float t) const=0;
        virtual std::list<CurveVertex<T> >	vertices(const float t1, const float t2) const=0;

        float                               min_vertex_distance;
        unsigned int                        max_vertices;

    protected:
        void                                fill_vertices(std::list<CurveVertex<T> > &vertices) const;
};



template <class T>
Curve<T>::Curve(const float min_vertex_distance)
:   min_vertex_distance(min_vertex_distance),
    max_vertices(static_cast<unsigned int>(std::pow(2, 16)-1))
{

}

template <class T>
Curve<T>::~Curve()
{

}

template <class T>
void Curve<T>::fill_vertices(std::list<CurveVertex<T> > &vertices) const
{
    if (std::isinf(this->min_vertex_distance))
        return;

    auto vertex = vertices.begin();
    while (vertex != std::prev(vertices.end()))
    {
        if (curve_vertex_distance(*vertex, *std::next(vertex)) > this->min_vertex_distance)
        {
            vertices.insert(std::next(vertex), this->vertex(0.5f*(vertex->t + std::next(vertex)->t)));
            if (vertices.size() > this->max_vertices)
                throw std::length_error("exceeded max number of vertices");
        }
        else
            vertex++;
    }
}

#endif
