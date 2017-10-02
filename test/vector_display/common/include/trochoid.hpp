#ifndef VECTOR_DISPLAY_TROCHOID_HPP
#define VECTOR_DISPLAY_TROCHOID_HPP

#include <list>
#include <glm/glm.hpp>
#include "parametric/curve.hpp"
#include "parametric/curve_support.hpp"

class Trochoid: public Curve<glm::vec2>
{
	public:
		Trochoid(const float stator_radius, const float rotor_radius, const float rotor_offset, const float min_vertex_distance);
		virtual ~Trochoid();

		virtual CurveVertex<glm::vec2>				vertex(const float t) const;
		virtual std::list<CurveVertex<glm::vec2> >	vertices(const float t1, const float t2) const;	

		float										stator_radius, rotor_radius, rotor_offset;
};

#endif
