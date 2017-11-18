#include <cmath>
#include <utility>
#include <algorithm>

#include "vector_display_curve.hpp"
#include "vector_display_curve_shaders.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/vertex_array_support.hpp"



VectorDisplayCurve::VectorDisplayCurve(const std::vector<glm::vec4> &vertices, Window &window, const std::string &vertex_array_name)
:   start_time(INFINITY),
	velocity(1.0f),
	color(1.0f),
    excitation_duration(0.001f),
    excitation(1.0f),
    excitation_time_constant(0.001f),
    decay_time_constant(0.05f),
    translation(0.0f),
    rotation(0.0f),
    width(0.01f),
    cap_height(0.005f),
	vertices(vertices),
	min_color_value(1.0f / 1024.0f),
	vertex_array_name(vertex_array_name),
    step_shader_name("vector_display_curve_step"),
    draw_shader_name("vector_display_curve_draw")
{
    window.add_vertex_array(this->vertex_array_name, DrawMode::points);
    window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec4>("vertices", vertices);
    window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec3>("vertex_colors", std::vector<glm::vec3>(vertices.size(), glm::vec3(0.0f)), BufferUsageFrequency::dynamic, BufferUsageAccess::draw);
}

VectorDisplayCurve::~VectorDisplayCurve()
{

}

glm::vec4 VectorDisplayCurve::get_first_vertex() const
{
	return this->vertices.front();
}

glm::vec4 VectorDisplayCurve::get_last_vertex() const
{
	return this->vertices.back();
}

std::string VectorDisplayCurve::get_vertex_array_name() const
{
	return this->vertex_array_name;
}

void VectorDisplayCurve::step(const Clock &clock, Window &window)
{
	window.get_shader(this->step_shader_name).set_attribute("vertex", window.get_vertex_array(this->vertex_array_name), "vertices");
	window.get_shader(this->step_shader_name).set_attribute("vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");
	window.get_shader(this->step_shader_name).set_transform_feedback_varying("next_vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");

	window.get_shader(this->step_shader_name).set_uniform("time", clock.time - (std::isinf(this->start_time) ? 0.0f : this->start_time));
	window.get_shader(this->step_shader_name).set_uniform("elapsed_time", std::isinf(this->start_time) ? 0.0f : clock.elapsed_time);
	window.get_shader(this->step_shader_name).set_uniform("velocity", this->velocity);

	window.get_shader(this->step_shader_name).set_uniform("color", this->color);
	window.get_shader(this->step_shader_name).set_uniform("excitation_duration", this->excitation_duration);
	window.get_shader(this->step_shader_name).set_uniform("excitation", this->excitation);
	window.get_shader(this->step_shader_name).set_uniform("excitation_time_constant", this->excitation_time_constant);
	window.get_shader(this->step_shader_name).set_uniform("decay_time_constant", this->decay_time_constant);

	window.get_shader(this->step_shader_name).set_uniform("min_color_value", this->min_color_value);

	window.get_vertex_array(this->vertex_array_name).draw_mode = DrawMode::points;
	window.get_vertex_array(this->vertex_array_name).draw(1, true);
}

void VectorDisplayCurve::draw(Window &window) const
{
    window.set_use_blending(true);
    window.set_blend_factors(BlendFactor::one, BlendFactor::one);

    window.get_shader(this->draw_shader_name).set_attribute("vertex", window.get_vertex_array(this->vertex_array_name), "vertices");
    window.get_shader(this->draw_shader_name).set_attribute("vertex_color", window.get_vertex_array(this->vertex_array_name), "vertex_colors");

    window.get_shader(this->draw_shader_name).set_uniform("translation", this->translation);
    window.get_shader(this->draw_shader_name).set_uniform("rotation", this->rotation);

    window.get_shader(this->draw_shader_name).set_uniform("width", this->width);
    window.get_shader(this->draw_shader_name).set_uniform("cap_height", this->cap_height);

    window.get_vertex_array(this->vertex_array_name).draw_mode = DrawMode::line_strip_adjacency;
    window.get_vertex_array(this->vertex_array_name).draw();
}

void initialize_window_vector_display_curve(Window &window, const std::string &step_shader_name, const std::string &draw_shader_name)
{
	window.add_shader(step_shader_name, {std::make_pair(vector_display_curve_step_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_curve_step_fragment_shader, ShaderType::fragment)}, {"next_vertex_color"});
	window.add_shader(draw_shader_name, {std::make_pair(vector_display_curve_draw_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_curve_draw_geometry_shader, ShaderType::geometry), std::make_pair(vector_display_curve_draw_fragment_shader, ShaderType::fragment)});
}
