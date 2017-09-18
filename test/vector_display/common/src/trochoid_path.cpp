#include <stdexcept>
#include <cmath>
#include <iterator>
#include <list>

#include "trochoid_path.hpp"



TrochoidPath::TrochoidPath(const float stator_radius, const float rotor_radius, const float rotor_offset, const float min_vertex_distance)
:	stator_radius(stator_radius),
	rotor_radius(rotor_radius),
	rotor_offset(rotor_offset),
	min_vertex_distance(min_vertex_distance),
	max_vertices(std::pow(2, 16)-1)
{

}

TrochoidPath::~TrochoidPath()
{

}

PathVertex<glm::vec2> TrochoidPath::vertex(const float t) const
{
    float c = std::sqrt((this->stator_radius - this->rotor_radius)*(this->stator_radius - this->rotor_radius)*(this->rotor_offset*this->rotor_offset + this->rotor_radius*this->rotor_radius) / (this->rotor_radius*this->rotor_radius));

	float x = (this->stator_radius - this->rotor_radius)*std::cos(t / c) + this->rotor_offset*std::cos((this->stator_radius - this->rotor_radius)*(t / c) / this->rotor_radius);
	float y = (this->stator_radius - this->rotor_radius)*std::sin(t / c) - this->rotor_offset*std::sin((this->stator_radius - this->rotor_radius)*(t / c) / this->rotor_radius);
	return PathVertex<glm::vec2>(glm::vec2(x, y), t);
}

std::list<PathVertex<glm::vec2> > TrochoidPath::vertices(const float t1, const float t2) const
{
	std::list<PathVertex<glm::vec2> > vertices{this->vertex(t1), this->vertex(t2)};

	auto vertex = vertices.begin();
	while (vertex != std::prev(vertices.end()))
	{
		if (glm::distance(vertex->vertex, std::next(vertex)->vertex) > this->min_vertex_distance)
		{
			vertices.insert(std::next(vertex), this->vertex(0.5f*(vertex->t + std::next(vertex)->t)));
			if (vertices.size() > this->max_vertices)
				throw std::length_error("exceeded max number of vertices");
		}
		else
			vertex++;
	}

	return vertices;
}
