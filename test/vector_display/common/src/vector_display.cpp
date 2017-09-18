#include <utility>
#include <iterator>
#include <algorithm>

#include "vector_display.hpp"
#include "vector_display_shaders.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/vertex_array_support.hpp"



VectorDisplay::VectorDisplay(Window &window, const std::array<unsigned int, 2> &size, const std::string &frame_buffer_name)
:   excitation_time_constant(0.01f),
    decay_time_constant_1(0.1f),
    decay_time_constant_2(10.0f),
    decay_threshold_1(1.0f),
    decay_threshold_2(0.5f / 255.0f),
    kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    beam_kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    glow_filter(window),
	excitation_shader_name("vector_display_excite"),
	decay_shader_name("vector_display_decay"),
	frame_buffer_name(frame_buffer_name),
	current_value_color_attachment(0),
	next_value_color_attachment(1),
	time(0.0f)
{
    std::vector<std::string> shader_names = window.get_shader_names();
    if (std::find(shader_names.begin(), shader_names.end(), this->excitation_shader_name) == shader_names.end())
        window.add_shader(this->excitation_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_excitation_fragment_shader, ShaderType::fragment)});
    if (std::find(shader_names.begin(), shader_names.end(), this->decay_shader_name) == shader_names.end())
        window.add_shader(this->decay_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_decay_fragment_shader, ShaderType::fragment)});

    std::vector<std::string> vertex_array_names = window.get_vertex_array_names();
    if (std::find(vertex_array_names.begin(), vertex_array_names.end(), this->vertex_array_name) == vertex_array_names.end())
    {
        window.add_vertex_array(this->vertex_array_name, DrawMode::triangle_strip);
        window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec2>("vertex_positions", {glm::vec2(-1.0, -1.0), glm::vec2(1.0, -1.0), glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0)});
    }

	window.add_frame_buffer(this->frame_buffer_name, size, 2, false);
	window.set_target_frame_buffer(this->frame_buffer_name, {0, 1});
	window.clear();

	this->max_shader_vertices = window.get_shader(this->excitation_shader_name).get_uniform_size("vertices");

	this->beam_kinetics.add_path(this->time);
	this->beam_kinetics.path_duration = 1.0f;

    this->glow_filter.size = 0.1f;
    this->glow_filter.width = 0.015f;
    this->glow_filter.amplitude = 1.0f;

    this->glow_filter.blend = true;
    this->glow_filter.source_blend_factor = BlendFactor::one;
    this->glow_filter.destination_blend_factor = BlendFactor::one;

    this->glow_filter.source_frame_buffer_name = this->frame_buffer_name;
    this->glow_filter.source_color_attachment = this->current_value_color_attachment;
    this->glow_filter.intermediate_frame_buffer_name = this->frame_buffer_name;
    this->glow_filter.intermediate_color_attachment = this->next_value_color_attachment;
    this->glow_filter.destination_frame_buffer_name = window.window_frame_buffer_name;
    this->glow_filter.destination_color_attachment = 0;
}

VectorDisplay::~VectorDisplay()
{

}

float VectorDisplay::get_time() const
{
    return this->time;
}

VectorDisplayPath& VectorDisplay::get_path(const std::string &name)
{
	return this->paths.at(name);
}

void VectorDisplay::add_path(const std::string &name, VectorDisplayPath &path)
{
	this->paths.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(path));
}

void VectorDisplay::remove_path(const std::string &name)
{
	this->paths.erase(name);
}

void VectorDisplay::step(const float elapsed_time, Window &window)
{
	this->kinetics.step(elapsed_time, this->time + elapsed_time);
	this->beam_kinetics.step(elapsed_time, this->time + elapsed_time);
	std::list<PathVertex<glm::vec2> > beam_vertices = this->beam_kinetics.get_path()->vertices(this->time, this->time + elapsed_time);

	window.set_use_blending(false);
	window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->current_value_color_attachment).set_texture_unit(0);
	window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->next_value_color_attachment).set_texture_unit(1);
	window.copy_frame_buffer(this->frame_buffer_name, this->current_value_color_attachment, this->frame_buffer_name, this->next_value_color_attachment);

	for (auto path = this->paths.begin(); path != this->paths.end(); ++path)
	{
		std::vector<std::vector<glm::vec4> > segments = path->second.pop_back(elapsed_time);
        for (auto segment = segments.begin(); segment != segments.end(); ++segment)
        {

            // merge vertices with beam path


            unsigned int begin_index = 0, end_index = 1;
            while (true)
            {
                begin_index = end_index-1;
                end_index = std::min(begin_index + this->max_shader_vertices, static_cast<unsigned int>(segment->size()));

                std::vector<glm::vec4> vertices(end_index - begin_index);
                std::copy(std::next(segment->begin(), begin_index), std::next(segment->begin(), end_index), vertices.begin());

                window.set_target_frame_buffer(this->frame_buffer_name, {this->next_value_color_attachment});
                window.get_shader(this->excitation_shader_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
                window.get_shader(this->excitation_shader_name).set_uniform("num_vertices", static_cast<int>(vertices.size()));
                window.get_shader(this->excitation_shader_name).set_uniform("vertices", vertices);
                window.get_shader(this->excitation_shader_name).set_uniform("value_sampler", 0);
                window.get_shader(this->excitation_shader_name).set_uniform("beam_width", path->second.width);
                window.get_shader(this->excitation_shader_name).set_uniform("time_constant", this->excitation_time_constant);
                window.get_shader(this->excitation_shader_name).set_uniform("beam_color", path->second.color);
                window.get_vertex_array(this->vertex_array_name).draw();

                if (end_index < segment->size())
                    window.copy_frame_buffer(this->frame_buffer_name, this->next_value_color_attachment, this->frame_buffer_name, this->current_value_color_attachment);
                else
                    break;
            }
        }
	}

	window.set_target_frame_buffer(this->frame_buffer_name, {this->current_value_color_attachment});
	window.get_shader(this->decay_shader_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
	window.get_shader(this->decay_shader_name).set_uniform("elapsed_time", elapsed_time);
	window.get_shader(this->decay_shader_name).set_uniform("value_sampler", 1);
	window.get_shader(this->decay_shader_name).set_uniform("time_constant_1", this->decay_time_constant_1);
	window.get_shader(this->decay_shader_name).set_uniform("threshold_1", this->decay_threshold_1);
	window.get_shader(this->decay_shader_name).set_uniform("time_constant_2", this->decay_time_constant_2);
	window.get_shader(this->decay_shader_name).set_uniform("threshold_2", this->decay_threshold_2);
	window.get_vertex_array(this->vertex_array_name).draw();

	this->time += elapsed_time;
}

void VectorDisplay::draw(Window &window)
{
    this->glow_filter.apply(window);
}
