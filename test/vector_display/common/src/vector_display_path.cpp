#include <utility>
#include <iterator>

#include "vector_display_path.hpp"



VectorDisplayPath::VectorDisplayPath(const std::shared_ptr<Path<glm::vec2> > &vertex_path, const std::shared_ptr<Path<bool> > &drawn_path, const float t, const float width, const float velocity, const glm::vec3 &color)
:	width(width),
    velocity(velocity),
    color(color),
	vertex_path(vertex_path),
	drawn_path(drawn_path),
	t(t)
{

}

VectorDisplayPath::~VectorDisplayPath()
{

}

std::tuple<std::vector<glm::vec4>, std::vector<int> > VectorDisplayPath::pop_back(const float dt, const std::shared_ptr<Path<glm::vec2> > &kinetic_path)
{
    std::vector<std::pair<float, float> > t1_t2s;
	if (this->drawn_path)
	{
		std::list<PathVertex<bool> > drawn_vertices = this->drawn_path->vertices(this->t, this->t + dt*this->velocity);

        auto drawn = drawn_vertices.begin();
        while (drawn != drawn_vertices.end())
        {
            if (drawn->vertex)
            {
                t1_t2s.push_back(std::make_pair(this->t + (drawn->t - this->t), this->t + (drawn->t - this->t)));

                while (drawn->vertex && drawn != drawn_vertices.end())
                    drawn++;

                t1_t2s.back().second = this->t + (drawn != drawn_vertices.end() ? drawn->t - this->t : dt*this->velocity);
            }
            else
                drawn++;
        }
	}
	else
		t1_t2s.push_back(std::make_pair(this->t, this->t + dt*this->velocity));

	std::vector<glm::vec4> vertices;
	std::vector<int> segment_vertices_indices;
    for (auto const &t1_t2 : t1_t2s)
    {
        if (segment_vertices_indices.size() == 0)
            segment_vertices_indices.push_back(0);

        std::list<PathVertex<glm::vec2> > vertex_vertices = this->vertex_path->vertices(t1_t2.first, t1_t2.second);
        auto vertex = vertex_vertices.begin();
        for (unsigned int i = 0; i < vertex_vertices.size(); i++)
        {
            if (kinetic_path)
                vertex->vertex += kinetic_path->vertex(vertex->t / this->velocity).vertex;

            vertices.push_back(glm::vec4(vertex->vertex, vertex->t / this->velocity, i == 0 ? 0.0f : vertices.back().w + glm::distance(vertex->vertex, std::prev(vertex)->vertex)));
            vertex++;
        }

        segment_vertices_indices.push_back(segment_vertices_indices.back() + vertex_vertices.size());
    }

    this->t += dt*this->velocity;

    return std::make_tuple(vertices, segment_vertices_indices);
}
