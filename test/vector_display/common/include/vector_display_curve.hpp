#ifndef VECTOR_DISPLAY_VECTOR_DISPLAY_CURVE_HPP
#define VECTOR_DISPLAY_VECTOR_DISPLAY_CURVE_HPP

#include <memory>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "parametric/curve.hpp"
#include "parametric/curve_support.hpp"

class VectorDisplayCurve
{
	public:
		VectorDisplayCurve(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &drawn_curve, const float velocity, const float offset_time,  const float width, const glm::vec3 &color);
		~VectorDisplayCurve();

		// void													set_velocity(const float velocity, const float continuity_time);

		std::tuple<std::vector<glm::vec4>, std::vector<int> >   shader_vertices(const Clock &clock, const std::shared_ptr<const Curve<glm::vec2> > &kinetic_history);

		float                                                   velocity, offset_time;

		float                                                   width;
		glm::vec3                                               color;

	private:
        float                                                   t_to_time(const float t) const;
        float                                                   time_to_t(const float time) const;
        
		std::shared_ptr<const Curve<glm::vec2> >                vertex_curve;
		std::shared_ptr<const Curve<bool> >                     drawn_curve;
};

#endif
