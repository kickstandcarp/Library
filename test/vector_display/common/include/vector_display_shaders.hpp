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

static const std::string vector_display_excitation_shader = R"(#version 330 core
layout(location = 0) out vec4   next_value;

in vec2                         position;

uniform int                     num_vertices;
uniform vec4[32]                vertices; // x, y, t, d
// uniform float[32]            drawn_vertices; // 1.0 if beam is drawn, 0.0 if not

uniform sampler2D               value_sampler;

uniform float                   beam_width, time_constant;
uniform vec3                    beam_color;

vec3                            dv_dt(float t, vec3 v);

void main()
{
    float t = vertices[0].z, d = vertices[0].w;
    float fragment_distance = distance(position, vertices[0].xy);
    for (int i = 0; i < num_vertices-1; i++)
    {
        float segment_length = vertices[i+1].w - vertices[i].w;
        vec2 segment_direction = mix(vec2(0.0), (vertices[i+1].xy - vertices[i].xy) / segment_length, bvec2(segment_length != 0.0));
        float projection_length = clamp(dot(position - vertices[i].xy, segment_direction), 0.0, segment_length);
		float projection_distance = distance(position, vertices[i].xy + projection_length*segment_direction);

		bool nearest = projection_distance < fragment_distance;
		t = mix(t, mix(vertices[i].z, vertices[i+1].z, projection_length / segment_length), nearest);
		d = mix(d, vertices[i].w + projection_length, nearest);
        fragment_distance = mix(fragment_distance, projection_distance, nearest);
    }
	
    if (fragment_distance > 5.0*beam_width)
		discard;

	float t1 = vertices[0].z, t2 = vertices[num_vertices-1].z;
    for (int i = 0; i < num_vertices-1; i++)
	{
		float segment_length = vertices[i+1].w - vertices[i].w;
		t1 = mix(t1, mix(vertices[i].z, vertices[i+1].z, (d - 5.0*beam_width - vertices[i].w) / (vertices[i+1].w - vertices[i].w)), d - 5.0*beam_width >= vertices[i].w && d - 5.0*beam_width <= vertices[i+1].w);
		t2 = mix(t2, mix(vertices[i].z, vertices[i+1].z, (d + 5.0*beam_width - vertices[i].w) / (vertices[i+1].w - vertices[i].w)), d + 5.0*beam_width >= vertices[i].w && d + 5.0*beam_width <= vertices[i+1].w);
	}

    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

	float dt = (t2 - t1) / 3.0;
    for (int i = 0; i < 3; i++)
    {
        vec3 k1 = dv_dt(t1, value);
        vec3 k2 = dv_dt(t1 + 0.5*dt, value + 0.5*dt*k1);
        vec3 k3 = dv_dt(t1 + 0.5*dt, value + 0.5*dt*k2);
        vec3 k4 = dv_dt(t1 + dt, value + dt*k3);
        value += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        t1 += dt;
    }

    next_value = vec4(value, 1.0);
}

vec3 dv_dt(float t, vec3 v)
{
    vec2 path_position = vertices[0].xy;
    for (int i = 0; i < num_vertices-1; i++)
    {
        float normalized_t = (t - vertices[i].z) / (vertices[i+1].z - vertices[i].z);
        path_position = mix(path_position, mix(vertices[i].xy, vertices[i+1].xy, normalized_t), bvec2(normalized_t >= 0.0 && normalized_t <= 1.0));
    }
	float fragment_distance = distance(position, path_position);

	vec3 excitation_v = beam_color*exp(-(fragment_distance*fragment_distance) / (2.0*beam_width*beam_width));

	return max(vec3(0.0), (excitation_v - v) / time_constant);
})";

static const std::string vector_display_decay_shader = R"()";

static const std::string vector_display_glow_1_shader = R"()";

static const std::string vector_display_glow_2_shader = R"()";

#endif
