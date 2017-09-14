#include <utility>
#include <algorithm>

#include "vector_display.hpp"
#include "vector_display_shaders.hpp"
#include "opengl/shader_support.hpp"
#include "opengl/vertex_array_support.hpp"



VectorDisplay::VectorDisplay(Window &window, const std::array<unsigned int, 2> &size)
:   excitation_time_constant(0.01f),
    decay_time_constant_1(0.1f),
    decay_time_constant_2(10.0f),
    decay_threshold_1(1.0f),
    decay_threshold_2(0.5f / 255.0f),
    kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
    beam_kinetics(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(1.0f)),
	glow_size(100),
	glow_width(15.0f),
	glow_intensity(3.0f),
	excitation_shader_name("vector_display_excite"),
	decay_shader_name("vector_display_decay"),
	glow_1_shader_name("vector_display_glow_1"),
	glow_2_shader_name("vector_display_glow_2"),
	frame_buffer_name("vector_display"),
	current_value_color_attachment(0),
	next_value_color_attachment(1),
	time(0.0f)
{
	window.add_shader(this->excitation_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_excitation_shader, ShaderType::fragment)});
	// window.add_shader(this->decay_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_decay_shader, ShaderType::fragment)});
	// window.add_shader(this->glow_1_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_glow_1_shader, ShaderType::fragment)});
	// window.add_shader(this->glow_2_shader_name, {std::make_pair(vector_display_vertex_shader, ShaderType::vertex), std::make_pair(vector_display_glow_2_shader, ShaderType::fragment)});

	window.add_vertex_array(this->vertex_array_name, DrawMode::triangle_strip);
	window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec2>("vertex_positions", {glm::vec2(-1.0, -1.0), glm::vec2(1.0, -1.0), glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0)});

	window.add_frame_buffer(this->frame_buffer_name, size, 2, false);
	window.set_target_frame_buffer(this->frame_buffer_name, {0, 1});
	window.clear();

	this->max_shader_vertices = window.get_shader(this->excitation_shader_name).get_uniform_size("vertices");

	this->beam_kinetics.add_path(this->time);
	this->beam_kinetics.path_duration = 1.0f;
}

VectorDisplay::~VectorDisplay()
{

}

VectorDisplayPath& VectorDisplay::get_path(const std::string &name)
{
	return this->paths.at(name).first;
}

void VectorDisplay::add_path(const std::string &name, VectorDisplayPath &path)
{
	this->paths.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(path, std::tuple<std::vector<glm::vec4>, std::vector<glm::vec2> >()));
}

void VectorDisplay::remove_path(const std::string &name)
{
	this->paths.erase(name);
}

void VectorDisplay::update(const float elapsed_time)
{
	this->kinetics.step(elapsed_time, this->time + elapsed_time);
	this->beam_kinetics.step(elapsed_time, this->time + elapsed_time);
		
	std::list<PathVertex<glm::vec2> > beam_vertices = this->beam_kinetics.get_path()->vertices(this->time, this->time + elapsed_time);

	for (auto &path : this->paths)
    {
		path.second.second = path.second.first.pop_vertices(elapsed_time);
		
		// merge_vertices
	}

	this->time += elapsed_time;
}

void VectorDisplay::draw(Window &window)
{
	window.set_use_blending(false);
	window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->current_value_color_attachment).set_texture_unit(0);
	window.get_frame_buffer(this->frame_buffer_name).get_color_texture(this->next_value_color_attachment).set_texture_unit(1);
	window.copy_frame_buffer(this->frame_buffer_name, this->current_value_color_attachment, this->frame_buffer_name, this->next_value_color_attachment);

	for (auto const &path : this->paths)
	{
		unsigned int begin_index = 0, end_index, size = static_cast<unsigned int>(std::get<0>(path.second.second).size());
		while (true)
		{
			end_index = std::min(begin_index + this->max_shader_vertices, size);

			std::vector<glm::vec4> vertices(end_index - begin_index);
			std::copy(std::next(std::get<0>(path.second.second).begin(), begin_index), std::next(std::get<0>(path.second.second).begin(), end_index), vertices.begin());

			window.set_target_frame_buffer(this->frame_buffer_name, {this->next_value_color_attachment});
			window.get_shader(this->excitation_shader_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
			window.get_shader(this->excitation_shader_name).set_uniform("num_vertices", static_cast<int>(vertices.size()));
			window.get_shader(this->excitation_shader_name).set_uniform("vertices", vertices);
			window.get_shader(this->excitation_shader_name).set_uniform("value_sampler", 0);
			window.get_shader(this->excitation_shader_name).set_uniform("beam_width", path.second.first.width);
			window.get_shader(this->excitation_shader_name).set_uniform("time_constant", this->excitation_time_constant);
			window.get_shader(this->excitation_shader_name).set_uniform("beam_color", path.second.first.color);
			window.get_vertex_array(this->vertex_array_name).draw();

			begin_index = end_index;

			if (end_index == size)
				break;

			/* if end_index != len(vertices):
				window.copy_frame_buffer("vector_display", this->next_value_color_attachment, "vector_display", this->current_value_color_attachment);

				if segment_index != len(segment_vertices)-1:
					window.copy_frame_buffer("vector_display", this->next_value_color_attachment, "vector_display", this->current_value_color_attachment);

					if path_index != len(self.paths)-1:
						window.copy_frame_buffer("vector_display", this->next_value_color_attachment, "vector_display", this->current_value_color_attachment); */
		}
	}

	/* window.set_target_frame_buffer(self.frame_buffer_name, [self.current_value_color_attachment])
	window.shaders(self.decay_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
	window.shaders(self.decay_shader_name).set_uniform('elapsed_time', elapsed_time)
	window.shaders(self.decay_shader_name).set_uniform('value_sampler', 1)
	window.shaders(self.decay_shader_name).set_uniform('time_constant_1', self.decay_time_constant_1)
	window.shaders(self.decay_shader_name).set_uniform('threshold_1', self.decay_threshold_1)
	window.shaders(self.decay_shader_name).set_uniform('time_constant_2', self.decay_time_constant_2)
	window.shaders(self.decay_shader_name).set_uniform('threshold_2', self.decay_threshold_2)
	window.vertex_arrays(self.vertex_array_name).draw() */
}
