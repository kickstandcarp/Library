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
layout(location = 0) out vec4   next_value;

in vec2                         texture_position;

uniform vec4[128]               vertices; // x, y, t, d
uniform int                     num_segments;
uniform int[65]                 segment_indices;

uniform sampler2D               value_sampler;
uniform float                   time, elapsed_time;
uniform float                   threshold;

uniform float                   excitation;
uniform vec3                    color;
uniform float                   width;
uniform float                   excitation_time_constant, excitation_decay_time_constant, decay_time_constant;

void                            transform_position(in vec2 position, out int segment_index, out float tangential_position, out float normal_position);
void                            excitation_integration_limits(in int segment_index, in float tangential_position, out int i1, out int i2, out float t1, out float t2);

vec3                            excite(in float t1, in float t2, in vec3 v, in vec2 position, in int i1, in int i2);
vec3                            excite_dv_dt(in float t, in vec3 v, in vec2 position, in int i1, in int i2);

vec3                            decay(in float t1, in float t2, in vec3 v);
vec3                            decay_dv_dt(in vec3 v);

vec3                            v_f(in float t, in vec2 position, in int i1, in int i2);
vec3							tau(in vec3 v, in vec3 v_f);



void main()
{
    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    vec2 position = 2.0*texture_position - 1.0;

    int segment_index;
    float tangential_position, normal_position;
    transform_position(position, segment_index, tangential_position, normal_position);

    if (all(equal(value, vec3(0.0))) && normal_position > 5.0*width)
		discard;

    int i1, i2;
    float t1, t2;
    excitation_integration_limits(segment_index, tangential_position, i1, i2, t1, t2);

    value = decay(time, t1, value);
    value = excite(t1, t2, value, position, i1, i2);
    value = decay(t2, time + elapsed_time, value);

    next_value = vec4(mix(vec3(0.0), value, step(vec3(threshold), value)), 1.0);
}

void transform_position(in vec2 position, out int segment_index, out float tangential_position, out float normal_position)
{
    segment_index = 0;
    tangential_position = vertices[0].w;
    normal_position = distance(position, vertices[0].xy);

    for (int i = 0; i < num_segments; i++)
    {
        for (int j = segment_indices[i]; j < segment_indices[i+1]-1; j++)
        {
            float segment_length = vertices[j+1].w - vertices[j].w;
            vec2 segment_direction = (vertices[j+1].xy - vertices[j].xy) / mix(1.0, segment_length, 1.0 - step(segment_length, 1.0e-6)*step(-1.0e-6, segment_length));
            float projection_length = clamp(dot(position - vertices[j].xy, segment_direction), 0.0, segment_length);
            float projection_distance = distance(position, vertices[j].xy + projection_length*segment_direction);

            float is_nearest = step(projection_distance, normal_position);
            segment_index = int(mix(float(segment_index), float(i), is_nearest));
            tangential_position = mix(tangential_position, vertices[j].w + projection_length, is_nearest);
            normal_position = mix(normal_position, projection_distance, is_nearest);
        }
    }
}

void excitation_integration_limits(in int segment_index, in float tangential_position, out int i1, out int i2, out float t1, out float t2)
{
    float d1 = tangential_position - 5.0*width, d2 = tangential_position + 5.0*width;

    i1 = segment_indices[segment_index], i2 = segment_indices[segment_index+1]-2;
	t1 = vertices[segment_indices[segment_index]].z, t2 = vertices[segment_indices[segment_index+1]-1].z;
    for (int i = segment_indices[segment_index]; i < segment_indices[segment_index+1]-1; i++)
	{
        float d1_in_segment = step(vertices[i].w, d1 + 1.0e-6)*step(d1 - 1.0e-6, vertices[i+1].w);
        float d2_in_segment = step(vertices[i].w, d2 + 1.0e-6)*step(d2 - 1.0e-6, vertices[i+1].w);

        i1 = int(mix(float(i1), float(i), d1_in_segment));
        i2 = int(mix(float(i2), float(i), d2_in_segment));

		float segment_length = vertices[i+1].w - vertices[i].w;
		segment_length = mix(1.0, segment_length, 1.0 - step(segment_length, 1.0e-6)*step(-1.0e-6, segment_length));
		t1 = mix(t1, mix(vertices[i].z, vertices[i+1].z, (d1 - vertices[i].w) / segment_length), d1_in_segment);
		t2 = mix(t2, mix(vertices[i].z, vertices[i+1].z, (d2 - vertices[i].w) / segment_length), d2_in_segment);
	}
}

vec3 excite(in float t1, in float t2, in vec3 v, in vec2 position, in int i1, in int i2)
{
	float t = t1, dt = (t2 - t1) / 5.0;

    for (int i = 0; i < 5; i++)
    {
        vec3 k1 = excite_dv_dt(t, v, position, i1, i2);
        vec3 k2 = excite_dv_dt(t + 0.5*dt, v + 0.5*dt*k1, position, i1, i2);
        vec3 k3 = excite_dv_dt(t + 0.5*dt, v + 0.5*dt*k2, position, i1, i2);
        vec3 k4 = excite_dv_dt(t + dt, v + dt*k3, position, i1, i2);
        v += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);

        t += dt;
    }

    return v;
}

vec3 excite_dv_dt(in float t, in vec3 v, in vec2 position, in int i1, in int i2)
{
	vec3 v_f = v_f(t, position, i1, i2);
	return (v_f - v) / tau(v, v_f);
}

vec3 decay(in float t1, in float t2, in vec3 v)
{
	float dt = t2 - t1;

    vec3 k1 = decay_dv_dt(v);
    vec3 k2 = decay_dv_dt(v + 0.5*dt*k1);
    vec3 k3 = decay_dv_dt(v + 0.5*dt*k2);
    vec3 k4 = decay_dv_dt(v + dt*k3);
    v += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);

	return v;
}

vec3 decay_dv_dt(in vec3 v)
{
	return -v / tau(v, vec3(0.0));
}

vec3 v_f(in float t, in vec2 position, in int i1, in int i2)
{
	vec2 xy = vertices[i1].xy;
	for (int i = i1; i <= i2; i++)
	{
		float t_in_segment = step(vertices[i].z, t)*step(t, vertices[i+1].z);

		float segment_duration = vertices[i+1].z - vertices[i].z;
		segment_duration = mix(1.0, segment_duration, 1.0 - step(segment_duration, 1.0e-6)*step(-1.0e-6, segment_duration));
		xy = mix(xy, mix(vertices[i].xy, vertices[i+1].xy, (t - vertices[i].z) / segment_duration), t_in_segment);
	}
	float beam_distance = distance(position, xy);

	return excitation*color*exp(-(beam_distance*beam_distance) / (2.0*width*width));
}

vec3 tau(in vec3 v, in vec3 v_f)
{
	vec3 time_constant = exp(mix(vec3(log(excitation_decay_time_constant)), vec3(log(decay_time_constant)), smoothstep(1.1*color, 0.9*color, v)));
	return mix(vec3(excitation_time_constant), time_constant, step(v_f, v));
})";

static const std::string vector_display_accumulation_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4   next_value;

in vec2                         texture_position;

uniform vec2                    translation;

uniform sampler2D               value_sampler;



void main()
{
    next_value = texture(value_sampler, texture_position - translation);
})";

#endif
