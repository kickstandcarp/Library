#ifndef EXPLODE_MODE_BACKGROUND_SHADERS_HPP
#define EXPLODE_MODE_BACKGROUND_SHADERS_HPP

#include <string>

static const std::string background_step_vertex_shader = R"(#version 330 core
layout(location = 0) in vec3    vertex;
layout(location = 1) in vec3    vertex_color;
layout(location = 2) in vec4    parameters; // angle, velocity, min_distance, max_distance

out vec3                        next_vertex, next_vertex_color;

uniform float                   elapsed_time;

uniform float                   radius;



void main()
{
    float distance = vertex.y + length(vertex.xz) + parameters[1]*elapsed_time;
    distance = mix(distance, parameters[2] + (distance - parameters[3]), distance > parameters[3]);

    next_vertex = min(distance, radius)*vec3(cos(parameters[0]), 0.0, sin(parameters[0])) + max(0.0, distance - radius)*vec3(0.0, 1.0, 0.0);
    next_vertex_color = vertex_color;
})";

static const std::string background_step_fragment_shader = R"(#version 330 core

void main()
{
    discard;
})";

static const std::string background_draw_vertex_shader = R"(#version 330 core
layout(location = 0) in vec3    vertex;
layout(location = 1) in vec3    vertex_color;

out vec3                        color;

uniform mat4x4                  camera_transform_matrix;



void main()
{
    color = vertex_color;

    gl_Position = camera_transform_matrix*vec4(vertex, 1.0);
})";

static const std::string background_draw_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   fragment_color;

in vec3                         color;



void main()
{
    fragment_color = vec4(color, 1.0);
})";

#endif
