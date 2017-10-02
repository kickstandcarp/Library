#include <utility>
#include <iterator>

#include "vector_display_curve.hpp"


#include <iostream>


VectorDisplayCurve::VectorDisplayCurve(const std::shared_ptr<const Curve<glm::vec2> > &vertex_curve, const std::shared_ptr<const Curve<bool> > &drawn_curve, const float velocity, const float offset_time, const float width, const glm::vec3 &color)
:	velocity(velocity),
    offset_time(offset_time),
    width(width),
    color(color),
	vertex_curve(vertex_curve),
	drawn_curve(drawn_curve)
{

}

VectorDisplayCurve::~VectorDisplayCurve()
{

}

/* void VectorDisplayCurve::set_velocity(const float velocity, const float continuity_time)
{
	this->velocity = velocity;
} */

std::tuple<std::vector<glm::vec4>, std::vector<int> > VectorDisplayCurve::shader_vertices(const Clock &clock, const std::shared_ptr<const Curve<glm::vec2> > &kinetic_history)
{
    std::vector<std::pair<float, float> > t1_t2s;
	if (this->drawn_curve)
	{
		std::list<CurveVertex<bool> > drawn_vertices = this->drawn_curve->vertices(this->time_to_t(clock.time), this->time_to_t(clock.time + clock.elapsed_time));

        auto drawn = drawn_vertices.begin();
        while (drawn != drawn_vertices.end())
        {
            if (drawn->value)
            {
                t1_t2s.push_back(std::make_pair(drawn->t, drawn->t));

                while (drawn->value && drawn != drawn_vertices.end())
                    drawn++;

                t1_t2s.back().second = drawn == drawn_vertices.end() ?  this->time_to_t(clock.time + clock.elapsed_time) : drawn->t;
            }
            else
                drawn++;
        }
	}
	else
		t1_t2s.push_back(std::make_pair(this->time_to_t(clock.time), this->time_to_t(clock.time + clock.elapsed_time)));

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
            if (kinetic_history)
                vertex->value += kinetic_history->vertex(this->t_to_time(vertex->t)).value;

            vertices.push_back(glm::vec4(vertex->value, this->t_to_time(vertex->t), i == 0 ? 0.0f : vertices.back().w + glm::distance(vertex->value, std::prev(vertex)->value)));
            vertex++;
        }

        segment_indices.push_back(segment_indices.back() + vertex_vertices.size());
    }

    return std::make_tuple(vertices, segment_indices);
}

float VectorDisplayCurve::t_to_time(const float t) const
{
    return (t / this->velocity) - this->offset_time;
}

float VectorDisplayCurve::time_to_t(const float time) const
{
    return this->velocity*(time + this->offset_time);
}
