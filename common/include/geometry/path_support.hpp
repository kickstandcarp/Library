#ifndef GEOMETRY_PATH_SUPPORT_HPP
#define GEOMETRY_PATH_SUPPORT_HPP

#include <limits>
#include <list>
// #include "geometry/path.hpp"

// template <class T> std::list<T>     merge_path_vertices(const std::list<T> &vertices_1, const std::list<T> &vertices_2);

// template <class T> std::list<T>     coninuous_path_vertices(const Path<T> &path, const float t1, const float t2, const float min_adjacent_vertex_distance, const unsigned int max_vertices=UINT_LEAST16_MAX);



/* template <class T>
std::list<T> merge_path_vertices(const std::list<T> &vertices_1, const std::list<T> &vertices_2)
{
    // Get all t values in both lists
    // Evaluate v1 + v2  at all t values
} */

/* template <class T>
std::list<T> coninuous_path_vertices(const Path<T> &path, const float t1, const float t2, const float min_adjacent_vertex_distance, const unsigned int max_vertices)
{
    std::list<T> path_vertices{path.vertex(t1), path.vertex(t2)};

    auto path_vertex = path_vertices.begin();
    while (path_vertex != std::prev(path_vertices.end()))
    {
        if (glm::distance(vertex->vertex, glm::vec2(*std::next(vertex))) > min_adjacent_vertex_distance)
        {
            vertices.insert(std::next(vertex), path.vertex(0.5f*(get_t(*vertex) + get_t(*std::next(vertex)))));
            if (vertices.size() > max_vertices)
                break;
        }
        else
            std::advance(vertex, 1);
    }

    return vertices;
} */

#endif
