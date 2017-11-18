#ifndef EXPLODE_MODE_SQUARE_SHADERS_HPP
#define EXPLODE_MODE_SQUARE_SHADERS_HPP

#include <string>

static const std::string square_vertex_shader = R"(#version 330 core
layout(location = 0) in vec3    vertex;

out vec3                        position, normal;
out float                       alpha;

uniform mat4x4                  camera_transform_matrix, transform_matrix;

uniform float                   dissolve_elapsed_time, dissolve_duration;



void main()
{
	position = vec3(transform_matrix*vec4(vertex, 1.0));
	normal = normalize(vec3(transform_matrix*vec4(0.0, 1.0, 0.0, 0.0)));

    alpha = 1.0 + (vertex.x + 0.5) - (dissolve_elapsed_time / dissolve_duration);

    gl_Position = camera_transform_matrix*vec4(position, 1.0);
})";

static const std::string square_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   fragment_color;

in vec3                         position, normal;
in float                        alpha;

uniform vec3                    camera_position;

uniform vec3                    color;
uniform float					reflectivity, emissivity;



void main()
{
	if (alpha < 1.0)
		discard;

    vec3 camera_direction = normalize(camera_position - position);
    vec3 point_source_direction = normalize(position - vec3(0.0, 0.0, 0.0));

    float point_source_diffuse_value = clamp(-dot(point_source_direction, normal), 0.0, 1.0);
    float point_source_specular_value = pow(clamp(dot(camera_direction, reflect(point_source_direction, normal)), 0.0, 1.0), 10.0);
    vec3 emissivity_reflectivity_color = color*(emissivity + reflectivity*(point_source_diffuse_value + point_source_specular_value));

	fragment_color = vec4(emissivity_reflectivity_color, 1.0);
})";

#endif
