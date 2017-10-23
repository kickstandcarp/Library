#ifndef VECTOR_DISPLAY_VECTOR_DISPLAY_SHADERS_HPP
#define VECTOR_DISPLAY_VECTOR_DISPLAY_SHADERS_HPP

#include <string>

static const std::string vector_display_vertex_shader = R"(#version 330 core
layout(location = 0) in vec2	vertex_position;

out vec2                        texture_position;



void main()
{
	texture_position = 0.5*(vertex_position + 1.0);
	gl_Position = vec4(vertex_position, 0.0, 1.0);
})";

static const std::string vector_display_beam_fragment_shader = R"(#version 330 core

const int                       max_segments = 8;

layout(location = 0) out vec4   next_value;

in vec2                         texture_position;

uniform int                     num_vertices;
uniform vec4[32]                vertices; // x, y, excitation, time

uniform float                   threshold;
uniform sampler2D               value_sampler;

uniform vec3					beam_color;
uniform float                   beam_radius;
uniform float                   excitation_time_constant, excitation_decay_time_constant, decay_time_constant;

vec3                            excite(in float t1, in float t2, in vec3 v, in vec2 fragment_position, in int segment_index, in int iterations);
vec3                            excite_dv_dt(in float t, in vec3 v, in vec2 fragment_position, in int segment_index);
vec3                            decay(in float t1, in float t2, in vec3 v, in int iterations);
vec3                            decay_dv_dt(in vec3 v);
vec3                            decay_tau(in vec3 v);



void main()
{
    vec2 fragment_position = 2.0*texture_position - 1.0;

    int segment_count = 0;
    int segment_indices[max_segments];
    float d1[max_segments], d2[max_segments], interval_begin_time[max_segments], interval_end_time[max_segments];
	for (int i = 0; i < max_segments; i++)
	{
		segment_indices[i] = -1;
		interval_begin_time[i] = -1.0;
		interval_end_time[i] = -1.0;
	}

    for (int i = 0; i < num_vertices-1; i++)
    {
        float segment_length = distance(vertices[i].xy, vertices[i+1].xy);
        vec2 segment_direction = (vertices[i+1].xy - vertices[i].xy) / mix(1.0, segment_length, segment_length != 0.0);
        float projection_length = dot(fragment_position - vertices[i].xy, segment_direction);
        float projection_distance = distance(fragment_position, vertices[i].xy + clamp(projection_length, 0.0, segment_length)*segment_direction);

        float time = mix(vertices[i].w, vertices[i+1].w, projection_length / segment_length);
        float beam_time = (beam_radius / segment_length)*(vertices[i+1].w - vertices[i].w);

        bool insert = segment_count < max_segments && projection_distance < 3.0*beam_radius && vertices[i].z >= threshold;

        segment_indices[segment_count] = int(mix(float(segment_indices[segment_count]), float(i), insert));
		interval_begin_time[segment_count] = mix(interval_begin_time[segment_count], clamp(time - beam_time, vertices[i].w, vertices[i+1].w), insert);
		interval_end_time[segment_count] = mix(interval_end_time[segment_count], clamp(time + beam_time, vertices[i].w, vertices[i+1].w), insert);

        segment_count += clamp(int(mix(0.0, 1.0, insert)), 0, max_segments);
    }

    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    if (all(lessThan(value, vec3(threshold))) && segment_count == 0)
        discard;

    float t0 = vertices[0].w;
    for (int i = 0; i < segment_count; i++)
    {
        float t1 = mix(t0, interval_begin_time[i], i < segment_count);
        float t2 = mix(t0, interval_end_time[i], i < segment_count);

        value = decay(t0, t1, value, 1);
        value = excite(t1, t2, value, fragment_position, segment_indices[i], 5);

        t0 = t2;
    }
    value = decay(t0, vertices[num_vertices-1].w, value, 1);

    next_value = vec4(value, 1.0);
}

vec3 excite(in float t1, in float t2, in vec3 v, in vec2 fragment_position, in int segment_index, in int iterations)
{
    float t = t1, dt = (t2 - t1) / float(iterations);
    for (int i = 0; i < iterations; i++)
    {
        vec3 k1 = excite_dv_dt(t, v, fragment_position, segment_index);
        vec3 k2 = excite_dv_dt(t + 0.5*dt, v + dt*0.5*k1, fragment_position, segment_index);
        vec3 k3 = excite_dv_dt(t + 0.5*dt, v + dt*0.5*k2, fragment_position, segment_index);
        vec3 k4 = excite_dv_dt(t + dt, v + dt*k3, fragment_position, segment_index);
        v += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        t += dt;
    }

    return v;
}

vec3 excite_dv_dt(in float t, in vec3 v, in vec2 fragment_position, in int segment_index)
{
    vec2 beam_position = mix(vertices[segment_index].xy, vertices[segment_index+1].xy, mix(0.0, (t - vertices[segment_index].w) / (vertices[segment_index+1].w - vertices[segment_index].w), vertices[segment_index].w != vertices[segment_index+1].w));
    float beam_distance = distance(fragment_position, beam_position);
    vec3 v_f = vertices[segment_index].z*beam_color*exp(-(beam_distance*beam_distance) / (2.0*beam_radius*beam_radius));

    vec3 tau = mix(decay_tau(v), vec3(excitation_time_constant), greaterThan(v_f, v));

    return (v_f - v) / tau;
}

vec3 decay(in float t1, in float t2, in vec3 v, in int iterations)
{
    float t = t1, dt = (t2 - t1) / float(iterations);
    for (int i = 0; i < iterations; i++)
    {
        vec3 k1 = decay_dv_dt(v);
        vec3 k2 = decay_dv_dt(v + dt*0.5*k1);
        vec3 k3 = decay_dv_dt(v + dt*0.5*k2);
        vec3 k4 = decay_dv_dt(v + dt*k3);
        v += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        t += dt;
    }

    return v;
}

vec3 decay_dv_dt(in vec3 v)
{
    return -v / decay_tau(v);
}

vec3 decay_tau(in vec3 v)
{
    return exp(mix(vec3(log(decay_time_constant)), vec3(log(excitation_decay_time_constant)), smoothstep(beam_color, 2.0*beam_color, v)));
})";

static const std::string vector_display_accumulation_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   value;

in vec2                         texture_position;

uniform sampler2D               value_sampler;



void main()
{
    value = texture(value_sampler, texture_position);
})";

#endif
