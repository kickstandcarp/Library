#ifndef TROCHOID_PATH_HPP
#define TROCHOID_PATH_HPP

#include <list>
#include <glm/glm.hpp>
#include "geometry/path.hpp"
#include "geometry/path_vertex.hpp"

class TrochoidPath: public Path<glm::vec2>
{
	public:
		TrochoidPath(const float stator_radius, const float rotor_radius, const float rotor_offset, const float min_vertex_distance);
		virtual ~TrochoidPath();

		virtual PathVertex<glm::vec2>				vertex(const float t) const;
		virtual std::list<PathVertex<glm::vec2> >	vertices(const float t1, const float t2) const;	

		float										stator_radius, rotor_radius, rotor_offset;

		float										min_vertex_distance;
		unsigned int								max_vertices;
};

#endif
