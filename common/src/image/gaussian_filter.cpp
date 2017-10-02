#include <string>
#include <vector>

#include "image/gaussian_filter.hpp"
#include "image/gaussian_filter_shaders.hpp"
#include "opengl/shader_support.hpp"



GaussianFilter::GaussianFilter(Window &window)
:   size(0.0f),
	width(1.0f),
    amplitude(1.0f),
    blend(true),
    source_blend_factor(BlendFactor::one),
    destination_blend_factor(BlendFactor::one),
    source_color_attachment_index(0),
    intermediate_color_attachment_index(0),
    destination_color_attachment_index(0),
    shader_1_name("gaussian_filter_1"),
    shader_2_name("gaussian_filter_2"),
    vertex_array_name("gaussian_filter")
{
    std::vector<std::string> shader_names = window.get_shader_names();
    if (std::find(shader_names.begin(), shader_names.end(), this->shader_1_name) == shader_names.end())
        window.add_shader(this->shader_1_name, {std::make_pair(gaussian_filter_vertex_shader, ShaderType::vertex), std::make_pair(gaussian_filter_fragment_shader_1, ShaderType::fragment)});
    if (std::find(shader_names.begin(), shader_names.end(), this->shader_2_name) == shader_names.end())
        window.add_shader(this->shader_2_name, {std::make_pair(gaussian_filter_vertex_shader, ShaderType::vertex), std::make_pair(gaussian_filter_fragment_shader_2, ShaderType::fragment)});

    std::vector<std::string> vertex_array_names = window.get_vertex_array_names();
    if (std::find(vertex_array_names.begin(), vertex_array_names.end(), this->vertex_array_name) == vertex_array_names.end())
    {
        window.add_vertex_array(this->vertex_array_name, DrawMode::triangle_strip);
        window.get_vertex_array(this->vertex_array_name).add_buffer<glm::vec2>("vertex_positions", {glm::vec2(-1.0, -1.0), glm::vec2(1.0, -1.0), glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0)});
    }
}

GaussianFilter::~GaussianFilter()
{

}

void GaussianFilter::apply(Window &window) const
{
    float intermediate_frame_buffer_size_y = static_cast<float>(this->intermediate_frame_buffer_name == window.window_frame_buffer_name ? window.get_size()[1] : window.get_frame_buffer(this->intermediate_frame_buffer_name).get_color_texture(this->intermediate_color_attachment_index).get_size()[1]);
    float destination_frame_buffer_size_x = static_cast<float>(this->destination_frame_buffer_name == window.window_frame_buffer_name ? window.get_size()[0] : window.get_frame_buffer(this->destination_frame_buffer_name).get_color_texture(this->destination_color_attachment_index).get_size()[0]);

    window.set_use_blending(false);

    window.set_target_frame_buffer(this->intermediate_frame_buffer_name, {this->intermediate_color_attachment_index});
    window.clear();
    window.get_shader(this->shader_1_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
    window.get_shader(this->shader_1_name).set_uniform("size", static_cast<int>(2.0f*this->size*intermediate_frame_buffer_size_y));
    window.get_shader(this->shader_1_name).set_uniform("width", 2.0f*this->width*intermediate_frame_buffer_size_y);
    window.get_shader(this->shader_1_name).set_uniform("amplitude", this->amplitude);
	window.get_frame_buffer(this->source_frame_buffer_name).get_color_texture(this->source_color_attachment_index).set_texture_unit(1);
    window.get_shader(this->shader_1_name).set_uniform("color_sampler", 1);
    window.get_vertex_array(this->vertex_array_name).draw();

    window.set_use_blending(this->blend);
    window.set_blend_factors(this->source_blend_factor, this->destination_blend_factor);

    window.set_target_frame_buffer(this->destination_frame_buffer_name, {this->destination_color_attachment_index});
    window.get_shader(this->shader_2_name).set_attribute("vertex_position", window.get_vertex_array(this->vertex_array_name), "vertex_positions");
    window.get_shader(this->shader_2_name).set_uniform("size", static_cast<int>(2.0f*this->size*destination_frame_buffer_size_x));
    window.get_shader(this->shader_2_name).set_uniform("width", 2.0f*this->width*destination_frame_buffer_size_x);
    window.get_frame_buffer(this->intermediate_frame_buffer_name).get_color_texture(this->intermediate_color_attachment_index).set_texture_unit(1);
    window.get_shader(this->shader_2_name).set_uniform("intermediate_color_sampler", 1);
    window.get_frame_buffer(this->source_frame_buffer_name).get_color_texture(this->source_color_attachment_index).set_texture_unit(2);
    window.get_shader(this->shader_2_name).set_uniform("source_color_sampler", 2);
    window.get_vertex_array(this->vertex_array_name).draw();
}
