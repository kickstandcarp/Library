#ifndef EXPLODE_MODE_VECTOR_DISPLAY_SHADERS_HPP
#define EXPLODE_MODE_VECTOR_DISPLAY_SHADERS_HPP

#include <string>

static const std::string vector_display_curve_step_vertex_shader = R"(#version 330 core
layout(location = 0) in vec4    vertex; // x, y, time, excitation
layout(location = 1) in vec3    vertex_color;

out vec3                        next_vertex_color;

uniform float                   time, elapsed_time;
uniform float                   velocity;

uniform vec3                    color;
uniform float                   excitation_duration, excitation;
uniform float                   excitation_time_constant, decay_time_constant;

uniform float					min_color_value;

vec3                            integrate(in float t1, in float t2, in vec3 v, in int iterations);
vec3                            dv_dt(in float t, in vec3 v);



void main()
{
    float vertex_time = vertex.z / velocity;
    float t1 = clamp(vertex_time - 5.0*excitation_duration, time, time + elapsed_time);
    float t2 = clamp(vertex_time + 5.0*excitation_duration, time, time + elapsed_time);

    next_vertex_color = integrate(time, t1, vertex_color, 1);
    next_vertex_color = integrate(t1, t2, next_vertex_color, 5);
    next_vertex_color = integrate(t2, time + elapsed_time, next_vertex_color, 1);

	next_vertex_color = mix(next_vertex_color, vec3(0.0), lessThan(next_vertex_color, vec3(min_color_value)));
}

vec3 integrate(in float t1, in float t2, in vec3 v, in int iterations)
{
    float t = t1, dt = (t2 - t1) / float(iterations);
    for (int i = 0; i < iterations; i++)
    {
        vec3 k1 = dv_dt(t, v);
        vec3 k2 = dv_dt(t + 0.5*dt, v + dt*0.5*k1);
        vec3 k3 = dv_dt(t + 0.5*dt, v + dt*0.5*k2);
        vec3 k4 = dv_dt(t + dt, v + dt*k3);
        v += (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        t += dt;
    }

    return v;
}

vec3 dv_dt(in float t, in vec3 v)
{
    float dt = t - (vertex.z / velocity);
	vec3 v_h = min(color, color*excitation);
    vec3 v_f = color*excitation*exp(-(dt*dt) / (2.0*excitation_duration*excitation_duration));
    v_f = mix(v_f, max(v_f, v_h), greaterThan(v, v_h - min_color_value));

    vec3 tau = mix(vec3(decay_time_constant), vec3(excitation_time_constant), greaterThan(v_f, v));

    return (v_f - v) / tau;
})";

static const std::string vector_display_curve_step_fragment_shader = R"(#version 330 core

void main()
{
    discard;
})";

static const std::string vector_display_curve_draw_vertex_shader = R"(#version 330 core
layout(location = 0) in vec4    vertex; // x, y, time, excitation
layout(location = 1) in vec3    vertex_color;

out float                       mid_segment;
out vec3                        geometry_color;

uniform vec2                    translation;
uniform float                   rotation;



void main()
{
    mid_segment = vertex.w;
    geometry_color = vertex_color;

    gl_Position = vec4(vec2(vertex.x*cos(rotation), vertex.y) + translation, 0.0, 1.0);
})";

static const std::string vector_display_curve_draw_geometry_shader = R"(#version 330 core
layout (lines_adjacency)                 in;
layout (triangle_strip, max_vertices=4)  out;

in float                                 mid_segment[];
in vec3                                  geometry_color[];

out vec3                                 color;

out float                                segment_length;
out vec2                                 segment_position;

uniform float                            width, cap_height;


void main()
{
    gl_Position.zw = vec2(0.0, 1.0);

    color = geometry_color[1];
    segment_length = distance(gl_in[1].gl_Position.xy, gl_in[2].gl_Position.xy);
    if (segment_length > 0.0 && mid_segment[1] > 0.0 && mid_segment[2] > 0.0)
    {
        vec2 tangent = normalize(vec2(gl_in[2].gl_Position.x - gl_in[1].gl_Position.x, gl_in[2].gl_Position.y - gl_in[1].gl_Position.y));
        vec2 normal = normalize(vec2(gl_in[1].gl_Position.y - gl_in[2].gl_Position.y, gl_in[2].gl_Position.x - gl_in[1].gl_Position.x));

        vec2 adjacent_normal_1 = normalize(vec2(gl_in[0].gl_Position.y - gl_in[1].gl_Position.y, gl_in[1].gl_Position.x - gl_in[0].gl_Position.x));
        vec2 extrusion_1 = mix(normal, normalize(0.5*(adjacent_normal_1 + normal)), bvec2(mid_segment[0] > 0.0));
        extrusion_1 *= mix(1.0, abs(1.0 / dot(extrusion_1, normal)), mid_segment[0] > 0.0);

        vec2 adjacent_normal_2 = normalize(vec2(gl_in[2].gl_Position.y - gl_in[3].gl_Position.y, gl_in[3].gl_Position.x - gl_in[2].gl_Position.x));
        vec2 extrusion_2 = mix(normal, normalize(0.5*(adjacent_normal_2 + normal)), bvec2(mid_segment[3] > 0.0));
        extrusion_2 *= mix(1.0, abs(1.0 / dot(extrusion_2, normal)), mid_segment[3] > 0.0);

        mat2x2 global_to_local = inverse(mat2x2(normal, tangent));
        vec2 local_tangent = global_to_local*tangent;
        vec2 local_extrusion_1 = global_to_local*extrusion_1;
        vec2 local_extrusion_2 = global_to_local*extrusion_2;

        gl_Position.xy = gl_in[1].gl_Position.xy + width*extrusion_1 - cap_height*tangent*step(mid_segment[0], 0.0);
        segment_position = vec2(0.0, 0.0) + width*local_extrusion_1 - cap_height*local_tangent*step(mid_segment[0], 0.0);
        EmitVertex();

        gl_Position.xy = gl_in[1].gl_Position.xy - width*extrusion_1 - cap_height*tangent*step(mid_segment[0], 0.0);
        segment_position = vec2(0.0, 0.0) - width*local_extrusion_1 - cap_height*local_tangent*step(mid_segment[0], 0.0);
        EmitVertex();

        gl_Position.xy = gl_in[2].gl_Position.xy + width*extrusion_2 + cap_height*tangent*step(mid_segment[3], 0.0);
        segment_position = vec2(0.0, segment_length) + width*local_extrusion_2 + cap_height*local_tangent*step(mid_segment[3], 0.0);
        EmitVertex();

        gl_Position.xy = gl_in[2].gl_Position.xy - width*extrusion_2 + cap_height*tangent*step(mid_segment[3], 0.0);
        segment_position = vec2(0.0, segment_length) - width*local_extrusion_2 + cap_height*local_tangent*step(mid_segment[3], 0.0);
        EmitVertex();

        EndPrimitive();
    }
})";

static const std::string vector_display_curve_draw_fragment_shader = R"(#version 330 core
layout(location = 0) out vec4    fragment_color;

in vec3                         color;

in float                        segment_length;
in vec2                         segment_position;

uniform float                   width, cap_height;



void main()
{
    float x = segment_position.x, y = mix(mix(0.0, segment_position.y, segment_position.y < 0.0), segment_position.y - segment_length, segment_position.y > segment_length);
    fragment_color = vec4(exp(-((x*x / (2.0*width*width / 9.0)) + (y*y / (2.0*cap_height*cap_height / 9.0))))*color, 1.0);
})";

#endif
