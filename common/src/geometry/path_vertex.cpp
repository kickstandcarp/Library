#include "geometry/path_vertex.hpp"



template <>
PathVertex<bool> mix<bool>(const PathVertex<bool> &path_vertex_1, const PathVertex<bool> &path_vertex_2, const float t)
{
	if (t == path_vertex_2.t)
        return PathVertex<bool>(path_vertex_2.vertex, t);
    else
        return PathVertex<bool>(path_vertex_1.vertex, t);
}

template <>
PathVertex<glm::quat> mix<glm::quat>(const PathVertex<glm::quat> &path_vertex_1, const PathVertex<glm::quat> &path_vertex_2, const float t)
{
	return PathVertex<glm::quat>(glm::slerp(path_vertex_1.vertex, path_vertex_2.vertex, (t - path_vertex_1.t) / (path_vertex_2.t - path_vertex_1.t)), t);
}
