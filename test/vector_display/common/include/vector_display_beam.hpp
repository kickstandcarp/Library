#ifndef VECTOR_DISPLAY_VECTOR_DISPLAY_BEAM_HPP
#define VECTOR_DISPLAY_VECTOR_DISPLAY_BEAM_HPP

#include <memory>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "parametric/curve.hpp"
#include "parametric/curve_support.hpp"

class VectorDisplayBeam
{
	public:
        VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const glm::vec3 &color, const float radius, const float velocity, const float t=0.0f);
        VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<float> > &excitation_curve, const glm::vec3 &color, const float radius, const float velocity, const float t=0.0f);
        VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &excitation_curve, const glm::vec3 &color, const float radius, const float velocity, const float t=0.0f);
		~VectorDisplayBeam();

		std::vector<std::vector<glm::vec4> >        shader_vertices(const Clock &clock, const unsigned int max_vertices);

        glm::vec3									color;
        float                                       excitation;
		float										radius;
        float										velocity;

        float										excitation_time_constant;
        float										excitation_decay_time_constant, decay_time_constant;

        std::shared_ptr<const Curve<glm::vec2> >    vertex_curve;
        std::shared_ptr<const Curve<bool> >         activation_curve;
        std::shared_ptr<const Curve<float> >        excitation_curve;

        float									    t;
};

#endif
