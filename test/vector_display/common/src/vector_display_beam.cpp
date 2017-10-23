#include <cmath>
#include <iterator>

#include "vector_display_beam.hpp"
#include "math/arithmatic.hpp"
#include "math/integration.hpp"
#include "parametric/segment_curve.hpp"



VectorDisplayBeam::VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const glm::vec3 &color, const float radius, const float velocity, const float t)
:	color(color),
    excitation(1.0f),
    radius(radius),
    velocity(velocity),
    excitation_time_constant(0.001f),
    excitation_decay_time_constant(0.05f),
    decay_time_constant(1.0f),
	vertex_curve(vertex_curve),
    activation_curve(std::make_shared<SegmentCurve<bool> >(std::list<CurveVertex<bool> >{CurveVertex<bool>{true, -INFINITY}, CurveVertex<bool>{true, INFINITY}}, CurveInterpolation::floor)),
    t(t)
{

}

VectorDisplayBeam::VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<float> > &excitation_curve, const glm::vec3 &color, const float radius, const float velocity, const float t)
:   VectorDisplayBeam(vertex_curve, color, radius, velocity, t)
{
    if (excitation_curve)
    {
        this->excitation_curve = excitation_curve;
        this->activation_curve.reset();
    }
}

VectorDisplayBeam::VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &activation_curve, const glm::vec3 &color, const float radius, const float velocity, const float t)
:   VectorDisplayBeam(vertex_curve, color, radius, velocity, t)
{
    if (activation_curve)
        this->activation_curve = activation_curve;
}


VectorDisplayBeam::~VectorDisplayBeam()
{

}

std::vector<std::vector<glm::vec4> > VectorDisplayBeam::shader_vertices(const Clock &clock, const unsigned int max_vertices)
{
	if (!this->vertex_curve || !(this->activation_curve || this->excitation_curve))
        return std::vector<std::vector<glm::vec4> >(1, {glm::vec4(0.0f, 0.0f, 0.0f, clock.time), glm::vec4(0.0f, 0.0f, 0.0f, clock.time + clock.elapsed_time)});

    std::list<CurveVertex<glm::vec2> > vertex_vertices = this->vertex_curve->vertices(this->t, this->t + clock.elapsed_time*this->velocity);
    std::list<CurveVertex<float> > excitation_vertices;
    if (this->excitation_curve)
        excitation_vertices = this->excitation_curve->vertices(this->t, this->t + clock.elapsed_time*this->velocity);
    else
    {
        std::list<CurveVertex<bool> > activation_vertices = this->activation_curve->vertices(this->t, this->t + clock.elapsed_time*this->velocity);
        for (auto const &vertex : activation_vertices)
            excitation_vertices.push_back(CurveVertex<float>{vertex.value ? 1.0f : 0.0f, vertex.t});
    }

    if (vertex_vertices.size() == 0 || excitation_vertices.size() == 0)
        return std::vector<std::vector<glm::vec4> >(1, {glm::vec4(0.0f, 0.0f, 0.0f, clock.time), glm::vec4(0.0f, 0.0f, 0.0f, clock.time + clock.elapsed_time)});

    if (vertex_vertices.back().t < this->t + clock.elapsed_time*this->velocity)
        vertex_vertices.push_back(CurveVertex<glm::vec2>{vertex_vertices.back().value, this->t + clock.elapsed_time*this->velocity});

	auto vertex = vertex_vertices.begin();
	auto excitation = excitation_vertices.begin();
	std::vector<std::vector<glm::vec4> > vertices_excitationses(1, std::vector<glm::vec4>());
	while (vertex != vertex_vertices.end() || excitation != excitation_vertices.end())
	{
		while ((excitation == excitation_vertices.end() || vertex->t <= excitation->t) && vertex != vertex_vertices.end() && vertices_excitationses.back().size() < max_vertices)
		{
            if (excitation->t == vertex->t)
            {
                vertices_excitationses.back().push_back(glm::vec4(vertex->value, this->excitation*excitation->value, clock.time + (vertex->t - this->t) / this->velocity));
                excitation++;
            }
            else
            {
                float excitation_value;
                if (this->excitation_curve)
                    excitation_value = this->excitation*this->excitation_curve->vertex(vertex->t).value;
                else
                    excitation_value = this->activation_curve->vertex(vertex->t).value ? this->excitation : 0.0f;
                vertices_excitationses.back().push_back(glm::vec4(vertex->value, excitation_value, clock.time + (vertex->t - this->t) / this->velocity));
            }
			vertex++;
		}

		while ((vertex == vertex_vertices.end() || excitation->t < vertex->t) && excitation != excitation_vertices.end() && vertices_excitationses.back().size() < max_vertices)
		{
            vertices_excitationses.back().push_back(glm::vec4(this->vertex_curve->vertex(excitation->t).value, this->excitation*excitation->value, clock.time + (excitation->t - this->t) / this->velocity));
			excitation++;
		}

		if (vertices_excitationses.back().size() == max_vertices)
			vertices_excitationses.push_back(std::vector<glm::vec4>(1, vertices_excitationses.back().back()));
	}

    this->t += clock.elapsed_time*this->velocity;

    return vertices_excitationses;
}

