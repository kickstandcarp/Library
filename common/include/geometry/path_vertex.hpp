#ifndef GEOMETRY_PATH_VERTEX_HPP
#define GEOMETRY_PATH_VERTEX_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

template <class T>
class PathVertex
{
	public:
		PathVertex<T>(const T &vertex, const float t);
        ~PathVertex<T>();

		T							vertex;
		float						t;
};

template <class T> bool				path_vertex_eq(const PathVertex<T> &a, const PathVertex<T> &b);
template <class T> bool				path_vertex_cmp(const PathVertex<T> &a, const PathVertex<T> &b);

template <class T> PathVertex<T>	mix(const PathVertex<T> &path_vertex_1, const PathVertex<T> &path_vertex_2, const float t);
template <> PathVertex<bool>		mix(const PathVertex<bool> &path_vertex_1, const PathVertex<bool> &path_vertex_2, const float t);
template <> PathVertex<glm::quat>	mix(const PathVertex<glm::quat> &path_vertex_1, const PathVertex<glm::quat> &path_vertex_2, const float t);



template <class T>
PathVertex<T>::PathVertex(const T &vertex, const float t)
:	vertex(vertex),
	t(t)
{

}

template <class T>
PathVertex<T>::~PathVertex()
{

}

template <class T>
bool path_vertex_eq(const PathVertex<T> &a, const PathVertex<T> &b)
{
	return a.t == b.t;
}

template <class T>
bool path_vertex_cmp(const PathVertex<T> &a, const PathVertex<T> &b)
{
	return a.t < b.t;
}

template <class T>
PathVertex<T> mix(const PathVertex<T> &path_vertex_1, const PathVertex<T> &path_vertex_2, const float t)
{
	return PathVertex<T>(glm::mix(path_vertex_1.vertex, path_vertex_2.vertex, (t - path_vertex_1.t) / (path_vertex_2.t - path_vertex_1.t)), t);
}

#endif
