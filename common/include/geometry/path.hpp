#ifndef GEOMETRY_PATH_HPP
#define GEOMETRY_PATH_HPP

#include <list>
#include "geometry/path_vertex.hpp"

template <class T>
class Path
{
	public:
		Path();
        virtual ~Path();
		
		virtual PathVertex<T>				vertex(const float t) const=0;
        virtual std::list<PathVertex<T> >	vertices(const float t1, const float t2) const=0;	
};



template <class T>
Path<T>::Path()
{

}

template <class T>
Path<T>::~Path()
{

}

#endif
