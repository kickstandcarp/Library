#include <utility>
#include <iterator>
#include <algorithm>
#include <tuple>

#include "vector_display.hpp"
#include "vector_display_shaders.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/vertex_array_support.hpp"



VectorDisplay::VectorDisplay(Window &window, const std::array<unsigned int, 2> &size)
:   threshold(0.5f / 255.0f),
	kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    beam_kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    glow_filter(window),
	size(size),
	beam_shader_name("vector_display_beam"),
	accumulation_shader_name("vector_display_accumulation"),
    vertex_array_name("vector_display"),
	frame_buffer_name("vector_display"),
	prev_value_color_attachment_index(0),
	next_value_color_attachment_index(1),
	accumulation_color_attachment_index(0),
	intermediate_color_attachment_index(1)
{
    std::vector<std::string> shader_names = window.get_shader_names();
    if (std::find(shader_names.begin(), shader_names.end(), this->beam_shader_name) == shader_names.end())
        window.add_shader(this->beam_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_beam_fragment_shader, ShaderType::fragment)});
	if (std::find(shader_names.begin(), shader_names.end(), this->accumulation_shader_name) == shader_names.end())
		window.add_shader(this->accumulation_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_accumulation_fragment_shader, ShaderType::fragment)});

	this->max_shader_vertices = window.get_shader(this->beam_shader_name).get_uniform_size("vertices");

    std::vector<std::string> vertex_array_names = window.get_vertex_array_names();
    if (std::find(vertex_array_names.begin(), vertex_array_names.end(), this->vertex_array_name) == vertex_array_names.end())
    {
        window.add_vertex_array(this->vertex_array_name, DrawMode::triangle_strip);
        window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec2>("vertex_positions", {glm::vec2(-1.0, -1.0), glm::vec2(1.0, -1.0), glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0)});
    }

    std::vector<std::string> frame_buffer_names = window.get_frame_buffer_names();
    if (std::find(frame_buffer_names.begin(), frame_buffer_names.end(), this->frame_buffer_name) == frame_buffer_names.end())
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

std::vector<std::string> VectorDisplay::get_beam_names() const
{
    std::vector<std::string> names;
    for (auto const &beam : this->beams)
        names.push_back(beam.first);
    return names;
}

VectorDisplayBeam& VectorDisplay::get_beam(const std::string &name)
{
	return this->beams.at(name);
}

void VectorDisplay::add_beam(const std::string &name, VectorDisplayBeam &beam, Window &window)
{
	this->beams.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(beam));
	window.add_texture(name, std::vector<float>(), this->size, TextureFormat::rgba, TextureInterpolation::nearest, TextureWrap::clamp);
	window.get_texture(name).set_data(std::vector<float>(window.get_texture(name).get_length(), 0.0f), window.get_texture(name).get_size());
}

void VectorDisplay::remove_beam(const std::string &name, Window &window)
{
	this->beams.erase(name);
	window.remove_texture(name);
}
void VectorDisplay::step(const Clock &clock, Window &window, const std::vector<std::string> &beam_names)
{
	this->kinetics.step(clock);
	this->beam_kinetics.step(clock);

    std::vector<std::string> step_beam_names = beam_names.size() == 0 ? this->get_beam_names() : beam_names;
	for (auto const &beam_name : step_beam_names)
	{
        std::vector<glm::vec4> vertices;
        std::vector<int> segment_indices;
        std::tie(vertices, segment_indices) = this->beams.at(beam_name).shader_vertices(clock, this->beam_kinetics.get_history());

        window.set_use_blending(false);
        window.get_frame_buffer(this->frame_buffer_name).swap_color_texture(this->prev_value_color_attachment_index, window.get_texture(beam_name));

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
            window.get_shader(this->beam_shader_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
            window.get_shader(this->beam_shader_name).set_uniform("vertices", iteration_vertices);
            window.get_shader(this->beam_shader_name).set_uniform("num_segments", static_cast<int>(iteration_segment_indices.size()-1));
            window.get_shader(this->beam_shader_name).set_uniform("segment_indices", iteration_segment_indices);
            window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->prev_value_color_attachment_index).set_texture_unit(1);
            window.get_shader(this->beam_shader_name).set_uniform("value_sampler", 1);
            window.get_shader(this->beam_shader_name).set_uniform("time", iteration_time);
            window.get_shader(this->beam_shader_name).set_uniform("elapsed_time", iteration_elapsed_time);
            window.get_shader(this->beam_shader_name).set_uniform("threshold", this->threshold);
            window.get_shader(this->beam_shader_name).set_uniform("excitation", this->beams.at(beam_name).excitation);
            window.get_shader(this->beam_shader_name).set_uniform("color", this->beams.at(beam_name).color);
            window.get_shader(this->beam_shader_name).set_uniform("width", this->beams.at(beam_name).width);
            window.get_shader(this->beam_shader_name).set_uniform("excitation_time_constant", this->beams.at(beam_name).excitation_time_constant);
            window.get_shader(this->beam_shader_name).set_uniform("excitation_decay_time_constant", this->beams.at(beam_name).excitation_decay_time_constant);
            window.get_shader(this->beam_shader_name).set_uniform("decay_time_constant", this->beams.at(beam_name).decay_time_constant);
            window.get_vertex_array(this->vertex_array_name).draw();

            if (end_index != static_cast<int>(vertices.size()))
            {
                std::swap(this->prev_value_color_attachment_index, this->next_value_color_attachment_index);
                iteration_time += iteration_elapsed_time;
            }
            else
                break;
        }

        window.get_frame_buffer(this->frame_buffer_name).swap_color_texture(this->next_value_color_attachment_index, window.get_texture(beam_name));
	}
}

void VectorDisplay::draw(Window &window)
{
    window.set_use_blending(true);
    window.set_blend_factors(BlendFactor::one, BlendFactor::one);
    window.set_target_frame_buffer(this->frame_buffer_name, {this->accumulation_color_attachment_index});
    window.clear();
    for (auto beam = this->beams.begin(); beam != this->beams.end(); ++beam)
    {
        window.get_texture(beam->first).set_texture_unit(1);
        window.get_shader(this->accumulation_shader_name).set_uniform("translation", std::get<0>(this->kinetics.values));
        window.get_shader(this->accumulation_shader_name).set_uniform("value_sampler", 1);
        window.get_vertex_array(this->vertex_array_name).draw();
    }

    this->glow_filter.apply(window);
}
