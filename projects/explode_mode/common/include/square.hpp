#ifndef EXPLODE_MODE_SQUARE_HPP
#define EXPLODE_MODE_SQUARE_HPP

#include <string>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "geometry/coordinate_transform.hpp"
#include "physics/paper_kinetics.hpp"
#include "opengl/window.hpp"
#include "opengl/camera.hpp"

class Square
{
	public:
		Square(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity);
		virtual ~Square();

		float									ray_intersection_distance(const glm::vec3 &ray_position, const glm::vec3 &ray_direction) const;
		
		void									explode(const int num_new_squares, const glm::vec3 &direction);
		void									dissolve();

		void									step(const Clock &clock);

		void									draw(const Camera &camera, Window &window) const;

		std::shared_ptr<CoordinateTransform>	coordinate_transform;
		PaperKinetics							kinetics;

		glm::vec3								color;
		float									reflectivity, emissivity;

	private:
		glm::vec3								intersection_size;

		bool									dissolving;
		float									dissolve_radius, dissolve_height;
		float									dissolve_elapsed_time, dissolve_duration;

		std::string								shader_name;
		std::string								vertex_array_name;
};

void											initialize_window_square(Window &window, const std::string &shader_name="square", const std::string &vertex_array_name="square");

#endif
