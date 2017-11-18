#include <random>

#include <glm/glm.hpp>

#include "background.hpp"
#include "background_shaders.hpp"



Background::Background(const float radius, const float height, const unsigned int num_vertices, Window &window, const std::string &vertex_array_name)
:   radius(radius),
    height(height),
    vertex_array_name(vertex_array_name),
    step_shader_name("background_step"),
    draw_shader_name("background_draw")
{
    std::random_device seed;
    std::default_random_engine generator(seed());
    std::uniform_real_distribution<float> uniform(0.0f, 1.0f);

	std::vector<glm::vec4> parameters(num_vertices);
    for (unsigned int i = 0; i < num_vertices; i++)
    {
        float angle = glm::two_pi<float>()*uniform(generator);
        float velocity = 10.0f*uniform(generator) + 10.0f;
        float min_distance = 5.0f*uniform(generator) + 5.0f;
        float max_distance = min_distance + 5.0f*uniform(generator) + 5.0f;

        parameters[i] = glm::vec4(angle, velocity, min_distance, max_distance);
    }

    window.add_vertex_array(vertex_array_name, DrawMode::points);
    window.get_vertex_array(vertex_array_name).add_buffer<glm::vec3>("vertices", std::vector<glm::vec3>(num_vertices, glm::vec3(0.0f)), BufferUsageFrequency::dynamic, BufferUsageAccess::draw);
    window.get_vertex_array(vertex_array_name).add_buffer<glm::vec3>("vertex_colors", std::vector<glm::vec3>(num_vertices, glm::vec3(1.0f)), BufferUsageFrequency::dynamic, BufferUsageAccess::draw);
    window.get_vertex_array(vertex_array_name).add_buffer<glm::vec4>("parameters", parameters);
}

Background::~Background()
{

}

std::string Background::get_vertex_array_name() const
{
    return this->vertex_array_name;
}

void Background::step(const Clock &clock, Window &window)
{
    window.get_shader(this->step_shader_name).set_attribute("vertex", window.get_vertex_array(this->vertex_array_name), "vertices");
    window.get_shader(this->step_shader_name).set_attribute("vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");
    window.get_shader(this->step_shader_name).set_attribute("parameters", window.get_vertex_array(this->vertex_array_name), "parameters");
    window.get_shader(this->step_shader_name).set_transform_feedback_varying("next_vertex", window.get_vertex_array(this->vertex_array_name), "vertices");
    window.get_shader(this->step_shader_name).set_transform_feedback_varying("next_vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");

    window.get_shader(this->step_shader_name).set_uniform("elapsed_time", clock.elapsed_time);
    window.get_shader(this->step_shader_name).set_uniform("radius", this->radius);

    window.get_vertex_array(this->vertex_array_name).draw(1, true);
}

void Background::draw(const Camera &camera, Window &window) const
{
    window.get_shader(this->draw_shader_name).set_attribute("vertex", window.get_vertex_array(this->vertex_array_name), "vertices");
    window.get_shader(this->draw_shader_name).set_attribute("vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");

    window.get_shader(this->draw_shader_name).set_uniform("camera_transform_matrix", camera.get_transform_matrix());

    window.get_vertex_array(this->vertex_array_name).draw();
}

void initialize_window_background(Window &window, const std::string &step_shader_name, const std::string &draw_shader_name)
{
    window.add_shader(step_shader_name, {std::make_pair(background_step_vertex_shader, ShaderType::vertex), std::make_pair(background_step_fragment_shader, ShaderType::fragment)}, {"next_vertex", "next_vertex_color"});
    window.add_shader(draw_shader_name, {std::make_pair(background_draw_vertex_shader, ShaderType::vertex), std::make_pair(background_draw_fragment_shader, ShaderType::fragment)});
}
