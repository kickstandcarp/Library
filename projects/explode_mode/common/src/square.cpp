#include <cmath>
#include <utility>

#include "square.hpp"
#include "square_shaders.hpp"



Square::Square(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity)
:	coordinate_transform(std::make_shared<CoordinateTransform>(position, orientation)),
	kinetics(position, orientation, velocity, angular_velocity, 10.0f, 0.1f, 0.1f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f)),
	color(1.0f, 0.0f, 0.0f),
	reflectivity(0.9f),
	emissivity(0.2f),
	intersection_size(glm::vec3(1.1f, 0.1f, 1.1f)),
	dissolving(false),
	dissolve_radius(10.0f),
	dissolve_height(0.0f),
	dissolve_elapsed_time(0.0f),
	dissolve_duration(3.0f),
	shader_name("square"),
	vertex_array_name("square")
{
	std::get<0>(this->kinetics.external_accelerations) = glm::vec3(0.0f, -9.8f, 0.0f);

	this->kinetics.min_step = 1.0e-3f;
	this->kinetics.relative_step_tolerance = {{1.0e-3f, 1.0e-3f}};
}

Square::~Square()
{

}

float Square::ray_intersection_distance(const glm::vec3 &ray_position, const glm::vec3 &ray_direction) const
{
	/* glm::vec3 position = this->coordinate_transform->transform_position(ray_position, false);
	glm::vec3 direction = this->coordinate_transform->transform_direction(ray_direction, false);

	float intersection_distance = std::numeric_limits<float>::infinity();
	for (int i = 0; i < 6; i++)
	{
		if (direction[i % 3] != 0.0f)
		{
			float distance = -(position[i % 3] + (i / 3 == 0 ? -1.0f : 1.0f)*this->intersection_size[i % 3]) / direction[i % 3];
            if (distance >= 0.0f && distance < intersection_distance)
            {
                glm::vec3 intersection_position = position + distance*direction;

                bool intersected = true;
                for (int j = 0; j < 3; j++)
                {
                    if (j != i % 3 && std::abs(intersection_position[j]) > this->intersection_size[j])
                        intersected = false;
                }

                if (intersected)
                    intersection_distance = distance;
            }
		}
	}

    return intersection_distance; */

	return 0.0f;
}

void Square::explode(const int num_new_squares, const glm::vec3 &direction)
{
	/* glm::vec3 position = this->coordinate_transform->transform_position(glm::vec3(0.0f), true);

    for (int i = 0; i <= num_new_squares; i++)
	{
        glm::vec3 velocity_axis;
        float velocity_axis_magnitude = 0.0f;
        while (velocity_axis_magnitude == 0.0f)
        {
            velocity_axis = vector_plane_projection(uniform_distribution(glm::vec3(-1.0f), glm::vec3(1.0f)), direction);
            velocity_axis_magnitude = glm::length(velocity_axis);
        }
        velocity_axis /= velocity_axis_magnitude;
        float velocity_angle = gaussian_distribution(0.0f, 0.5f*PI);
		glm::vec3 velocity_direction = rotate(direction, velocity_axis, velocity_angle);

		glm::vec3 initial_velocity = 25.0f*velocity_direction;
		glm::vec3 initial_rotational_velocity(0.0f);

		if (i == 0)
		{
			this->kinetics->velocity = initial_velocity;
			this->kinetics->rotational_velocity = initial_rotational_velocity;

			this->time_kinetics->velocity = 1.0f;
			this->time_kinetics->value->set(1.0f);
		}
		else
		{
			glm::vec3 normal = vector_plane_projection(glm::vec3(0.0f, 1.0f, 0.0f), velocity_direction);
			float normal_magnitude = glm::length(normal);
			normal = normal_magnitude == 0.0f ? glm::normalize(uniform_distribution(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f))) : normal / normal_magnitude;
			glm::vec4 normal_axis_angle = rotation_axis_angle(glm::vec3(0.0f, 1.0f, 0.0f), normal);

			glm::vec3 rotation = glm::eulerAngles(glm::angleAxis(normal_axis_angle.w, glm::vec3(normal_axis_angle))) + uniform_distribution(glm::vec3(-0.025f*PI, 0.0f, -0.025f*PI), glm::vec3(0.025f*PI, 0.0f, 0.025f*PI));

			float color_value = uniform_distribution(0.0f, 1.0f);

			this->added_entities.push_back(std::make_shared<Square>(position, rotation, initial_velocity, initial_rotational_velocity, color_value, this->color_map, this->radius, this->clock));
		}
	} */
}


void Square::dissolve()
{
	this->dissolving = true;
	this->color = glm::vec3(1.0f);
	this->reflectivity = 0.0f;
	this->emissivity = 1.0f;
}

void Square::step(const Clock &clock)
{
	this->kinetics.step(clock);
	this->coordinate_transform->set_translation(std::get<0>(this->kinetics.values));
	this->coordinate_transform->set_rotation(std::get<1>(this->kinetics.values));

	if (this->dissolving)
		this->dissolve_elapsed_time += clock.elapsed_time;
}

void Square::draw(const Camera &camera, Window &window) const
{
	glm::mat4x4 transform_matrix = camera.get_transform_matrix()*this->coordinate_transform->get_global_matrix();

	float min_x = INFINITY, min_y = INFINITY, max_x = -INFINITY, max_y = -INFINITY;
	for (int i = 0; i < 4; i++)
	{
		glm::vec4 position = transform_matrix*glm::vec4((i % 2) == 0 ? -1.0f : 1.0f, 0.0, (i / 2) == 0 ? -1.0f : 1.0f, 1.0f);
		position /= position.w;

		min_x = std::min(min_x, position.x);
		min_y = std::min(min_y, position.y);
		max_x = std::max(max_x, position.x);
		max_y = std::max(max_y, position.y);
	}

	if (min_x < 1.0f && min_y < 1.0f && max_x > -1.0f && max_y > -1.0f)
	{
		window.set_use_blending(false);

		window.get_shader(this->shader_name).set_attribute("vertex", window.get_vertex_array(this->vertex_array_name), "vertices");

		window.get_shader(this->shader_name).set_uniform("camera_transform_matrix", camera.get_transform_matrix());
		window.get_shader(this->shader_name).set_uniform("transform_matrix", this->coordinate_transform->get_global_matrix());

		window.get_shader(this->shader_name).set_uniform("camera_position", camera.get_coordinate_transform()->transform_position(camera.get_coordinate_transform()->get_origin_translation(), true));

		window.get_shader(this->shader_name).set_uniform("color", this->color);
		window.get_shader(this->shader_name).set_uniform("reflectivity", this->reflectivity);
		window.get_shader(this->shader_name).set_uniform("emissivity", this->emissivity);

		window.get_shader(this->shader_name).set_uniform("dissolve_elapsed_time", this->dissolve_elapsed_time);
		window.get_shader(this->shader_name).set_uniform("dissolve_duration", this->dissolve_duration);

		window.get_vertex_array(this->vertex_array_name).draw();
	}
}

void initialize_window_square(Window &window, const std::string &shader_name, const std::string &vertex_array_name)
{
	window.add_shader(shader_name, {std::make_pair(square_vertex_shader, ShaderType::vertex), std::make_pair(square_fragment_shader, ShaderType::fragment)});

	window.add_vertex_array(vertex_array_name, DrawMode::triangle_strip);
	window.get_vertex_array(vertex_array_name).add_buffer<glm::vec3>("vertices", {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.0f, 0.5f)});
}
