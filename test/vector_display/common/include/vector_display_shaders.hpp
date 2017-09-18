#ifndef VECTOR_DISPLAY_SHADERS_HPP
#define VECTOR_DISPLAY_SHADERS_HPP

#include <string>

static const std::string vector_display_vertex_shader = R"(#version 330 core
layout(location = 0) in vec2	vertex_position;

out vec2                        position;

void main()
{
	position = vertex_position;
	gl_Position = vec4(vertex_position, 0.0, 1.0);
})";

static const std::string vector_display_excitation_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   next_value;

in vec2                         position;

uniform int                     num_vertices;
uniform vec4[32]                vertices; // x, y, t, d

uniform sampler2D               value_sampler;

uniform float                   beam_width, time_constant;
uniform vec3                    beam_color;

vec3                            dv_dt(float t, vec3 v, int i1, int i2);

void main()
{
    float d = vertices[0].w;
    float fragment_distance = distance(position, vertices[0].xy);
    for (int i = 0; i < num_vertices-1; i++)
    {
        float segment_length = vertices[i+1].w - vertices[i].w;
        vec2 segment_direction = mix(vec2(0.0), (vertices[i+1].xy - vertices[i].xy) / segment_length, bvec2(segment_length != 0.0));
        float projection_length = clamp(dot(position - vertices[i].xy, segment_direction), 0.0, segment_length);
		float projection_distance = distance(position, vertices[i].xy + projection_length*segment_direction);

		bool is_nearest = projection_distance < fragment_distance;
		d = mix(d, vertices[i].w + projection_length, is_nearest);
        fragment_distance = mix(fragment_distance, projection_distance, is_nearest);
    }
	
    if (fragment_distance > 5.0*beam_width)
		discard;

    float d1 = d - 5.0*beam_width, d2 = d + 5.0*beam_width;

    int i1 = 0, i2 = num_vertices-2;
	float t1 = vertices[0].z, t2 = vertices[num_vertices-1].z;
    for (int i = 0; i < num_vertices-1; i++)
	{
        bool d1_in_segment = d1 >= vertices[i].w && d1 <= vertices[i+1].w;
        bool d2_in_segment = d2 >= vertices[i].w && d2 <= vertices[i+1].w;

        i1 = int(mix(float(i1), float(i), d1_in_segment));
        i2 = int(mix(float(i2), float(i), d2_in_segment));
		t1 = mix(t1, mix(vertices[i].z, vertices[i+1].z, (d1 - vertices[i].w) / (vertices[i+1].w - vertices[i].w)), d1_in_segment);
		t2 = mix(t2, mix(vertices[i].z, vertices[i+1].z, (d2 - vertices[i].w) / (vertices[i+1].w - vertices[i].w)), d2_in_segment);
	}

    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

	float t = t1, dt = (t2 - t1) / 3.0;
    for (int i = 0; i < 3; i++)
    {
        vec3 k1 = dv_dt(t, value, i1, i2);
        vec3 k2 = dv_dt(t + 0.5*dt, value + 0.5*dt*k1, i1, i2);
        vec3 k3 = dv_dt(t + 0.5*dt, value + 0.5*dt*k2, i1, i2);
        vec3 k4 = dv_dt(t + dt, value + dt*k3, i1, i2);
        value += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        t += dt;
    }

    next_value = vec4(value, 1.0);
}

vec3 dv_dt(float t, vec3 v, int i1, int i2)
{
    vec2 path_position = vertices[i1].xy;
    for (int i = i1; i <= i2; i++)
    {
        float normalized_t = (t - vertices[i].z) / (vertices[i+1].z - vertices[i].z);
        path_position = mix(path_position, mix(vertices[i].xy, vertices[i+1].xy, normalized_t), bvec2(normalized_t >= 0.0 && normalized_t <= 1.0));
    }
	float fragment_distance = distance(position, path_position);

	vec3 excitation_v = beam_color*exp(-(fragment_distance*fragment_distance) / (2.0*beam_width*beam_width));

	return max(vec3(0.0), (excitation_v - v) / time_constant);
})";

static const std::string vector_display_decay_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   next_value;

in vec2                         position;

uniform float                   elapsed_time;
uniform sampler2D               value_sampler;

uniform float                   time_constant_1, threshold_1, time_constant_2, threshold_2;

void main()
{
    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    if (all(equal(value, vec3(0.0))))
        discard;

    vec3 k1 = (threshold_1 - value) / time_constant_1;
    vec3 k2 = (threshold_1 - (value + 0.5*elapsed_time*k1)) / time_constant_1;
    vec3 k3 = (threshold_1 - (value + 0.5*elapsed_time*k2)) / time_constant_1;
    vec3 k4 = (threshold_1 - (value + elapsed_time*k3)) / time_constant_1;
    vec3 value_1 = value + (elapsed_time / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);

    k1 = -value / time_constant_2;
    k2 = -(value + 0.5*elapsed_time*k1) / time_constant_2;
    k3 = -(value + 0.5*elapsed_time*k2) / time_constant_2;
    k4 = -(value + elapsed_time*k3) / time_constant_2;
    vec3 value_2 = value + (elapsed_time / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);

    value = mix(value_1, value_2, lessThan(value_1, vec3(1.01*threshold_1)));
    next_value = vec4(mix(value, vec3(0.0), lessThan(value.rgb, vec3(threshold_2))), 1.0);
})";

#endif
