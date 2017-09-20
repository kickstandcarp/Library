#ifndef IMAGE_GAUSSIAN_FILTER_SHADERS_HPP
#define IMAGE_GAUSSIAN_FILTER_SHADERS_HPP

#include <string>

static const std::string gaussian_filter_vertex_shader = R"(#version 330 core
layout(location = 0) in vec2	vertex_position;

out vec2                        position;



void main()
{
	position = vertex_position;
	gl_Position = vec4(vertex_position, 0.0, 1.0);
})";

static const std::string gaussian_filter_fragment_shader_1 = R"(#version 330 core
layout(location = 0) out vec4   fragment_color;

uniform int                     size;
uniform float                   width, amplitude;

uniform sampler2D               color_sampler;



void main()
{
    vec4 color = vec4(0.0);
    float normalization = 0.0;

    ivec2 texture_position = ivec2(gl_FragCoord.xy);
    for (int i = 0; i < size; i++)
    {
        int offset = i - (size / 2);
        float coefficient = exp(-float(offset*offset) / (2.0*width*width));

        vec4 texture_color = texelFetch(color_sampler, texture_position + ivec2(0, offset), 0);
        color += coefficient*texture_color;

        normalization += coefficient;
    }

    fragment_color = amplitude*color / normalization;
})";

static const std::string gaussian_filter_fragment_shader_2 = R"(#version 330 core
layout(location = 0) out vec4   fragment_color;

uniform int                     size;
uniform float                   width;

uniform sampler2D               intermediate_color_sampler, source_color_sampler;



void main()
{
    vec4 color = vec4(0.0);
    float normalization = 0.0;

    ivec2 texture_position = ivec2(gl_FragCoord.xy);
    for (int i = 0; i < size; i++)
    {
        int offset = i - (size / 2);
        float coefficient = exp(-float(offset*offset) / (2.0*width*width));

        vec4 texture_color = texelFetch(intermediate_color_sampler, texture_position + ivec2(offset, 0), 0);
        color += coefficient*texture_color;

        normalization += coefficient;
    }

    fragment_color = (color / normalization) + texelFetch(source_color_sampler, texture_position, 0);
})";


#endif
