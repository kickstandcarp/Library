#version 330 core

layout(location = 0) out vec4   next_value;

in float                        vertex_scan_distance;

uniform int                     iterations;

uniform float                   elapsed_time, scan_distance;
uniform float                   beam_width, beam_velocity, beam_intensity, time_constant;

uniform sampler2D               current_value_sampler;

vec3                            dv_dt(float time, vec3 value, float distance);



void main()
{
    vec3 current_value = texelFetch(current_value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    float interval_elapsed_time = elapsed_time / float(iterations);
    float interval_scan_distance = scan_distance;
    for (int i = 0; i < iterations; i++)
    {
        float distance = vertex_scan_distance - interval_scan_distance;

        vec3 k1 = dv_dt(0.0, current_value, distance);
        vec3 k2 = dv_dt(0.5*interval_elapsed_time, current_value + 0.5*interval_elapsed_time*k1, distance);
        vec3 k3 = dv_dt(0.5*interval_elapsed_time, current_value + 0.5*interval_elapsed_time*k2, distance);
        vec3 k4 = dv_dt(interval_elapsed_time, current_value + interval_elapsed_time*k3, distance);

        current_value += (interval_elapsed_time / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
        interval_scan_distance += interval_elapsed_time*beam_velocity;
    }

    next_value = vec4(current_value, 1.0);
}

vec3 dv_dt(float time, vec3 value, float distance)
{
    float d = distance - time*beam_velocity;
    float forcing_value = beam_intensity*exp(-(d*d) / (2.0*beam_width*beam_width));

    return max(vec3(0.0), (vec3(forcing_value) - value) / time_constant);
}
