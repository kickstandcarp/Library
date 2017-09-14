#version 330 core

layout(location = 0) out vec4   next_value;

in vec2                         position;

uniform int                     num_vertices;
uniform vec3[128]               vertices; // x, y, distance

uniform int                     iterations;
// uniform float                   scan_distance, elapsed_time;
uniform sampler2D               value_sampler;

uniform float                   beam_width, beam_length, beam_velocity, beam_intensity, time_constant;
uniform vec3                    beam_color;

vec2                            fragment_path_distance();

vec2                            path_position(float d);
vec3                            beam_value(float d, float path_distance);
vec3                            dv_dt(float d, vec3 v, float path_distance);



void main()
{
    vec2 temp = fragment_path_distance();

    float path_distance = temp.x;
    if (num_vertices == 0 || path_distance > 5.0*beam_width)
        discard;

    vec3 value = texelFetch(value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    // float interval_scan_distance = scan_distance;
    // float interval_elapsed_time = elapsed_time / float(iterations);

    float interval_scan_distance = max(vertices[0].z, temp.y - 5.0*beam_width);
    float interval_elapsed_time = ((min(vertices[num_vertices-1].z, temp.y + 5.0*beam_width) - interval_scan_distance) / beam_velocity) / float(iterations);

    for (int i = 0; i < iterations; i++)
    {
        vec3 k1 = dv_dt(interval_scan_distance, value, path_distance);
        vec3 k2 = dv_dt(interval_scan_distance + 0.5*interval_elapsed_time*beam_velocity, value + 0.5*interval_elapsed_time*k1, path_distance);
        vec3 k3 = dv_dt(interval_scan_distance + 0.5*interval_elapsed_time*beam_velocity, value + 0.5*interval_elapsed_time*k2, path_distance);
        vec3 k4 = dv_dt(interval_scan_distance + interval_elapsed_time*beam_velocity, value + interval_elapsed_time*k3, path_distance);
        value += (interval_elapsed_time / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);

        interval_scan_distance += interval_elapsed_time*beam_velocity;
    }

    next_value = vec4(value, 1.0);
}

vec2 fragment_path_distance()
{
    float path_position = vertices[0].z;
    float path_distance = distance(position, vertices[0].xy);
    for (int i = 0; i < num_vertices-1; i++)
    {
        float line_length = distance(vertices[i+1].xy, vertices[i].xy);
        vec2 line_direction = mix(vec2(0.0), (vertices[i+1].xy - vertices[i].xy) / line_length, bvec2(line_length != 0.0));
        float projection_length = clamp(dot(position - vertices[i].xy, line_direction), 0.0, line_length);
        float projection_distance = distance(position, vertices[i].xy + projection_length*line_direction);

        bool on_segment = projection_distance < path_distance;
        path_position = mix(path_position, mix(vertices[i].z, vertices[i+1].z, projection_length / line_length), on_segment);
        path_distance = mix(path_distance, projection_distance, on_segment);
    }

    return vec2(path_distance, path_position);
}

vec2 path_position(float d)
{
    vec2 vertex = vertices[0].xy;
    for (int i = 0; i < num_vertices-1; i++)
    {
        float a = (d - vertices[i].z) / (vertices[i+1].z - vertices[i].z);
        vertex = mix(vertex, mix(vertices[i].xy, vertices[i+1].xy, a), bvec2(a >= 0.0 && a <= 1.0));
    }

    return vertex;
}

vec3 beam_value(float d, float path_distance)
{
    float r = distance(position, path_position(d));
    return beam_intensity*beam_color*exp(-(r*r) / (2.0*beam_width*beam_width));
}

vec3 dv_dt(float d, vec3 v, float path_distance)
{
    return max(vec3(0.0), (beam_value(d, path_distance) - v) / time_constant);
}
