#include <iterator>

#include "vector_display_path.hpp"



VectorDisplayPath::VectorDisplayPath(const std::shared_ptr<Path<glm::vec2> > &path, const float t, const float width, const float velocity, const glm::vec3 &color)
:	width(width),
    velocity(velocity),
    color(color),
	t(t),
	vertex_path(path)
{

}

VectorDisplayPath::~VectorDisplayPath()
{

}

std::tuple<std::vector<glm::vec4>, std::vector<glm::vec2> > VectorDisplayPath::pop_vertices(const float dt)
{
	std::tuple<std::vector<glm::vec4>, std::vector<glm::vec2> > vertices;

    std::list<PathVertex<glm::vec2> > vertex_vertices = this->vertex_path->vertices(this->t, this->t + dt*this->velocity);
	auto vertex = vertex_vertices.begin();

	std::get<0>(vertices).resize(vertex_vertices.size());
	for (unsigned int i = 0; i < vertex_vertices.size(); i++)
	{
		std::get<0>(vertices)[i] = glm::vec4(vertex->vertex, vertex->t / this->velocity, i == 0 ? 0.0f : std::get<0>(vertices)[i-1].w + glm::distance(vertex->vertex, std::prev(vertex)->vertex));
		std::advance(vertex, 1);
	}

	if (this->drawn_path)
	{
		std::list<PathVertex<bool> > drawn_vertices = this->drawn_path->vertices(this->t, this->t + dt*this->velocity);
		auto drawn = drawn_vertices.begin();

		std::get<1>(vertices).resize(drawn_vertices.size());
		for (unsigned int i = 0; i < drawn_vertices.size(); i++)
		{
			std::get<1>(vertices)[i] = glm::vec2(drawn->vertex ? 1.0f : 0.0f, drawn->t / this->velocity);
			std::advance(drawn, 1);
		}
	}
	else
		std::get<1>(vertices) = {glm::vec2(1.0f, this->t), glm::vec2(1.0f, this->t + dt*this->velocity)};

    this->t += dt*this->velocity;

    return vertices;
}
