#ifndef VECTOR_DISPLAY_PATH_HPP
#define VECTOR_DISPLAY_PATH_HPP

#include <memory>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "geometry/path.hpp"
#include "geometry/path_vertex.hpp"

class VectorDisplayPath
{
	public:
		VectorDisplayPath(const std::shared_ptr<Path<glm::vec2> > &path, const float t, const float width, const float velocity, const glm::vec3 &color);
		~VectorDisplayPath();

		std::tuple<std::vector<glm::vec4>, std::vector<glm::vec2> >	pop_vertices(const float dt);

		float														width, velocity;
		glm::vec3													color;

	private:
		std::shared_ptr<Path<glm::vec2> >							vertex_path;
		std::shared_ptr<Path<bool> >								drawn_path;
		float														t;
};

#endif
