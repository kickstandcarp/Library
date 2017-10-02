#include <utility>
#include <iterator>
#include <algorithm>
#include <tuple>

#include "vector_display.hpp"
#include "vector_display_shaders.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/vertex_array_support.hpp"



VectorDisplay::VectorDisplay(Window &window, const std::array<unsigned int, 2> &size, const std::string &frame_buffer_name)
:   threshold(0.5f / 255.0f),
	excitation_time_constant(0.01f),
    decay_time_constant_1(0.1f),
	decay_time_constant_2(1.0f),
	decay_time_constant_edge_1(1.333f),
	decay_time_constant_edge_2(1.0f),
	kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    beam_kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    glow_filter(window),
	size(size),
	curve_shader_name("vector_display_curve"),
	accumulation_shader_name("vector_display_accumulation"),
    vertex_array_name("vector_display"),
	frame_buffer_name(frame_buffer_name),
	prev_value_color_attachment_index(0),
	next_value_color_attachment_index(1),
	accumulation_color_attachment_index(0),
	intermediate_color_attachment_index(1)
{
    std::vector<std::string> shader_names = window.get_shader_names();
    if (std::find(shader_names.begin(), shader_names.end(), this->curve_shader_name) == shader_names.end())
        window.add_shader(this->curve_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_curve_fragment_shader, ShaderType::fragment)});
	if (std::find(shader_names.begin(), shader_names.end(), this->accumulation_shader_name) == shader_names.end())
		window.add_shader(this->accumulation_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_accumulation_fragment_shader, ShaderType::fragment)});

	this->max_shader_vertices = window.get_shader(this->curve_shader_name).get_uniform_size("vertices");

    std::vector<std::string> vertex_array_names = window.get_vertex_array_names();
    if (std::find(vertex_array_names.begin(), vertex_array_names.end(), this->vertex_array_name) == vertex_array_names.end())
    {
        window.add_vertex_array(this->vertex_array_name, DrawMode::triangle_strip);
        window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec2>("vertex_positions", {glm::vec2(-1.0, -1.0), glm::vec2(1.0, -1.0), glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0)});
    }

	window.add_frame_buffer(this->frame_buffer_name, size, {TextureFormat::rgba, TextureFormat::rgba}, false);

	this->beam_kinetics.add_history(0.0f);
	this->beam_kinetics.history_duration = 1.0f;

    this->glow_filter.size = 0.1f;
    this->glow_filter.width = 0.015f;
    this->glow_filter.amplitude = 3.0f;

    this->glow_filter.blend = true;
    this->glow_filter.source_blend_factor = BlendFactor::one;
    this->glow_filter.destination_blend_factor = BlendFactor::one;

    this->glow_filter.source_frame_buffer_name = this->frame_buffer_name;
	this->glow_filter.source_color_attachment_index = this->accumulation_color_attachment_index;
	this->glow_filter.intermediate_frame_buffer_name = this->frame_buffer_name;
	this->glow_filter.intermediate_color_attachment_index = this->intermediate_color_attachment_index;
	this->glow_filter.destination_frame_buffer_name = window.window_frame_buffer_name;
    this->glow_filter.destination_color_attachment_index = 0;
}

VectorDisplay::~VectorDisplay()
{

}

VectorDisplayCurve& VectorDisplay::get_curve(const std::string &name)
{
	return this->curves.at(name);
}

void VectorDisplay::add_curve(const std::string &name, VectorDisplayCurve &curve, Window &window)
{
	this->curves.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(curve));
	window.add_texture(name, std::vector<float>(), this->size, TextureFormat::rgba, TextureInterpolation::nearest, TextureWrap::clamp);
	window.get_texture(name).set_data(std::vector<float>(window.get_texture(name).get_length(), 0.0f), window.get_texture(name).get_size());
}

void VectorDisplay::remove_curve(const std::string &name, Window &window)
{
	this->curves.erase(name);
	window.remove_texture(name);
}

void VectorDisplay::step(const Clock &clock, Window &window)
{
	this->kinetics.step(clock);
	this->beam_kinetics.step(clock);

	for (auto curve = this->curves.begin(); curve != this->curves.end(); ++curve)
	{
		std::vector<glm::vec4> vertices;
        std::vector<int> segment_indices;
		std::tie(vertices, segment_indices) = curve->second.shader_vertices(clock, this->beam_kinetics.get_history());

		window.set_use_blending(false);
		window.get_frame_buffer(this->frame_buffer_name).swap_color_texture(this->prev_value_color_attachment_index, window.get_texture(curve->first));

        float iteration_time = clock.time, iteration_elapsed_time;
        int begin_index, end_index = 1;
        while (true)
        {
            begin_index = end_index-1;
            end_index = std::min(begin_index + static_cast<int>(this->max_shader_vertices), static_cast<int>(vertices.size()));

            std::vector<glm::vec4> iteration_vertices;
			std::vector<int> iteration_segment_indices;
			if (begin_index == end_index)
			{
				iteration_vertices = {glm::vec4(0.0f, 0.0f, iteration_time, 0.0f), glm::vec4(0.0f, 0.0f, iteration_time, 0.0f)};
				iteration_segment_indices = {0, 2};
			}
			else
			{
				iteration_vertices.resize(end_index - begin_index);
				std::copy(std::next(vertices.begin(), begin_index), std::next(vertices.begin(), end_index), iteration_vertices.begin());

				iteration_segment_indices.push_back(0);
				auto iteration_segment_indices_begin = std::lower_bound(segment_indices.begin(), segment_indices.end(), begin_index);
				auto iteration_segment_indices_end = std::lower_bound(segment_indices.begin(), segment_indices.end(), end_index);
				for (auto segment_vertices_index = iteration_segment_indices_begin; segment_vertices_index != iteration_segment_indices_end; ++segment_vertices_index)
				{
					if (*segment_vertices_index > begin_index  && *segment_vertices_index < end_index)
						iteration_segment_indices.push_back(*segment_vertices_index - begin_index);
				}
				iteration_segment_indices.push_back(end_index - begin_index);
			}

			iteration_elapsed_time = end_index == static_cast<int>(vertices.size()) ? clock.time + clock.elapsed_time - iteration_time : iteration_vertices.back().z - iteration_time;

            window.set_target_frame_buffer(this->frame_buffer_name, {this->next_value_color_attachment_index});
			window.clear();
            window.get_shader(this->curve_shader_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
            window.get_shader(this->curve_shader_name).set_uniform("vertices", iteration_vertices);
            window.get_shader(this->curve_shader_name).set_uniform("num_segments", static_cast<int>(iteration_segment_indices.size()-1));
            window.get_shader(this->curve_shader_name).set_uniform("segment_indices", iteration_segment_indices);
            window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->prev_value_color_attachment_index).set_texture_unit(1);
            window.get_shader(this->curve_shader_name).set_uniform("value_sampler", 1);
            window.get_shader(this->curve_shader_name).set_uniform("time", iteration_time);
            window.get_shader(this->curve_shader_name).set_uniform("elapsed_time", iteration_elapsed_time);
			window.get_shader(this->curve_shader_name).set_uniform("threshold", this->threshold);
			window.get_shader(this->curve_shader_name).set_uniform("excitation_time_constant", this->excitation_time_constant);
            window.get_shader(this->curve_shader_name).set_uniform("decay_time_constant_1", this->decay_time_constant_1);
			window.get_shader(this->curve_shader_name).set_uniform("decay_time_constant_2", this->decay_time_constant_2);
			window.get_shader(this->curve_shader_name).set_uniform("decay_time_constant_edge_1", this->decay_time_constant_edge_1);
			window.get_shader(this->curve_shader_name).set_uniform("decay_time_constant_edge_2", this->decay_time_constant_edge_2);
			window.get_shader(this->curve_shader_name).set_uniform("beam_width", curve->second.width);
            window.get_shader(this->curve_shader_name).set_uniform("beam_color", curve->second.color);
			window.get_vertex_array(this->vertex_array_name).draw();

			if (end_index != static_cast<int>(vertices.size()))
            {
                std::swap(this->prev_value_color_attachment_index, this->next_value_color_attachment_index);
                iteration_time += iteration_elapsed_time;
            }
            else
                break;
        }

		window.get_frame_buffer(this->frame_buffer_name).swap_color_texture(this->next_value_color_attachment_index, window.get_texture(curve->first));
	}

	window.set_use_blending(true);
	window.set_blend_factors(BlendFactor::one, BlendFactor::one);
	window.set_target_frame_buffer(this->frame_buffer_name, {this->accumulation_color_attachment_index});
	window.clear();
	for (auto curve = this->curves.begin(); curve != this->curves.end(); ++curve)
	{
		window.get_texture(curve->first).set_texture_unit(1);
		window.get_shader(this->accumulation_shader_name).set_uniform("value_sampler", 1);
		window.get_vertex_array(this->vertex_array_name).draw();
	}
}

void VectorDisplay::draw(Window &window)
{
    this->glow_filter.apply(window);
}
