#include <utility>
#include <iterator>

#include "vector_display_beam.hpp"



VectorDisplayBeam::VectorDisplayBeam(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &drawn_curve, const float excitation, const glm::vec3 &color, const float width, const float velocity, const float t)
:	excitation(excitation),
    color(color),
    width(width),
    velocity(velocity),
    excitation_time_constant(0.001f),
    excitation_decay_time_constant(0.05f),
    decay_time_constant(1.0f),
	vertex_curve(vertex_curve),
	drawn_curve(drawn_curve),
    t(t)
{

}

VectorDisplayBeam::~VectorDisplayBeam()
{

}

std::tuple<std::vector<glm::vec4>, std::vector<int> > VectorDisplayBeam::shader_vertices(const Clock &clock, const std::shared_ptr<const Curve<glm::vec2> > &kinetic_history)
{
    if (!this->vertex_curve)
        return std::make_tuple(std::vector<glm::vec4>(), std::vector<int>());

    std::vector<std::pair<float, float> > t1_t2s;
	if (this->drawn_curve)
	{
		std::list<CurveVertex<bool> > drawn_vertices = this->drawn_curve->vertices(this->t, this->t + this->velocity*clock.elapsed_time);

        auto drawn = drawn_vertices.begin();
        while (drawn != drawn_vertices.end())
        {
            if (drawn->value)
            {
                t1_t2s.push_back(std::make_pair(drawn->t, drawn->t));

                while (drawn->value && drawn != drawn_vertices.end())
                    drawn++;

                t1_t2s.back().second = drawn == drawn_vertices.end() ? this->t + this->velocity*clock.elapsed_time : drawn->t;
            }
            else
                drawn++;
        }
	}
	else
		t1_t2s.push_back(std::make_pair(this->t, this->t + this->velocity*clock.elapsed_time));

	std::vector<glm::vec4> vertices;
	std::vector<int> segment_indices;
    for (auto const &t1_t2 : t1_t2s)
    {
        if (segment_indices.size() == 0)
            segment_indices.push_back(0);

        std::list<CurveVertex<glm::vec2> > vertex_vertices = this->vertex_curve->vertices(t1_t2.first, t1_t2.second);
        auto vertex = vertex_vertices.begin();
        for (unsigned int i = 0; i < vertex_vertices.size(); i++)
        {
            float vertex_time = clock.time + (vertex->t - this->t) / this->velocity;
            if (kinetic_history)
                vertex->value += kinetic_history->vertex(vertex_time).value;

            vertices.push_back(glm::vec4(vertex->value, vertex_time, i == 0 ? 0.0f : vertices.back().w + glm::distance(vertex->value, std::prev(vertex)->value)));
            vertex++;
        }

        segment_indices.push_back(segment_indices.back() + vertex_vertices.size());
    }

    this->t += clock.elapsed_time*this->velocity;

    return std::make_tuple(vertices, segment_indices);
}
