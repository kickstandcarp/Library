#ifndef VECTOR_DISPLAY_VECTOR_DISPLAY_CURVE_HPP
#define VECTOR_DISPLAY_VECTOR_DISPLAY_CURVE_HPP

#include <memory>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "parametric/curve.hpp"
#include "parametric/curve_support.hpp"

class VectorDisplayBeam
{
	public:
		VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &drawn_curve, const float excitation, const glm::vec3 &color, const float width, const float velocity, const float t=0.0f);
		~VectorDisplayBeam();

		std::tuple<std::vector<glm::vec4>, std::vector<int> >   shader_vertices(const Clock &clock, const std::shared_ptr<const Curve<glm::vec2> > &kinetic_history);

        float                                                   excitation;
        glm::vec3                                               color;
		float                                                   width;
        float                                                   velocity;

        float                                                   excitation_time_constant;
        float                                                   excitation_decay_time_constant, decay_time_constant;

        std::shared_ptr<const Curve<glm::vec2> >				vertex_curve;
        std::shared_ptr<const Curve<bool> >						drawn_curve;

        float                                                   t;
};

#endif
