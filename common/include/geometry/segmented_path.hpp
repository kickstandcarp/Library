#ifndef GEOMETRY_SEGMENTED_PATH_HPP
#define GEOMETRY_SEGMENTED_PATH_HPP

#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <list>
#include "geometry/path.hpp"
#include "geometry/path_vertex.hpp"

template <class T>
class SegmentedPath: public Path<T>
{
	public:
		SegmentedPath(const std::list<PathVertex<T> > &path_vertices);
        virtual ~SegmentedPath();

		const std::list<PathVertex<T> >&	get_path_vertices() const;

		void								add_path_vertex(const PathVertex<T> &path_vertex);

		void								remove_path_vertices_prior(const float t);
		
		virtual PathVertex<T>				vertex(const float t) const;
        virtual std::list<PathVertex<T> >	vertices(const float t1, const float t2) const;
	
	private:
		std::list<PathVertex<T> >			path_vertices;
};



template <class T>
SegmentedPath<T>::SegmentedPath(const std::list<PathVertex<T> > &path_vertices)
:	path_vertices(path_vertices)
{
	this->path_vertices.sort(&path_vertex_cmp<T>);
	this->path_vertices.unique(&path_vertex_eq<T>);
}

template <class T>
SegmentedPath<T>::~SegmentedPath()
{

}

template <class T>
const std::list<PathVertex<T> >& SegmentedPath<T>::get_path_vertices() const
{
	return this->path_vertices;
}

template <class T>
void SegmentedPath<T>::add_path_vertex(const PathVertex<T> &path_vertex)
{
	auto insert_position = this->path_vertices.end();
	if (!path_vertex_cmp(this->path_vertices.back(), path_vertex))
		insert_position = std::lower_bound(this->path_vertices.begin(), this->path_vertices.end(), path_vertex, &path_vertex_cmp<T>);

	this->path_vertices.insert(insert_position, path_vertex);

	if (path_vertex_eq(path_vertex, *insert_position))
		this->path_vertices.erase(insert_position);
}

template <class T>
void SegmentedPath<T>::remove_path_vertices_prior(const float t)
{
	auto path_vertex = this->path_vertices.begin();
	while (path_vertex != std::prev(this->path_vertices.end()))
	{
		if (std::next(path_vertex)->t <= t)
			path_vertex++;
		else
			break;
	}
	
	this->path_vertices.erase(this->path_vertices.begin(), path_vertex);
}

template <class T>
PathVertex<T> SegmentedPath<T>::vertex(const float t) const
{
	PathVertex<T> vertex(this->path_vertices.front().vertex, t);

	auto end_path_vertex = std::prev(this->path_vertices.end());
	for (auto path_vertex = this->path_vertices.begin(); path_vertex != end_path_vertex; ++path_vertex)
	{
		auto next_path_vertex = std::next(path_vertex);
		if ((path_vertex == this->path_vertices.begin() && t < path_vertex->t) || (t >= path_vertex->t && t <= next_path_vertex->t) || (next_path_vertex == end_path_vertex && t > next_path_vertex->t))
		{
			vertex = mix(*path_vertex, *next_path_vertex, t);
			break;
		}
	}

	return vertex;
}

template <class T>
std::list<PathVertex<T> > SegmentedPath<T>::vertices(const float t1, const float t2) const
{
	std::list<PathVertex<T> > vertices;
	for (auto path_vertex = this->path_vertices.begin(); path_vertex != this->path_vertices.end(); ++path_vertex)
	{
		if (path_vertex->t > t1 && path_vertex->t < t2)
			vertices.push_back(*path_vertex);
	}

	if (t1 >= this->path_vertices.front().t && t1 <= this->path_vertices.back().t)
        vertices.push_front(this->vertex(t1));

	if (t2 >= this->path_vertices.front().t && t2 <= this->path_vertices.back().t)
        vertices.push_back(this->vertex(t2));

	return vertices;
}

#endif
