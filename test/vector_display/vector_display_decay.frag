#version 330 core

layout(location = 0) out vec4   next_value;

uniform float                   elapsed_time;
uniform float                   time_constant;

uniform sampler2D	        current_value_sampler;



void main()
{
    vec3 current_value = texelFetch(current_value_sampler, ivec2(gl_FragCoord.xy), 0).rgb;

    if (all(lessThan(current_value, vec3(1.1920929e-7))))
        discard;

    vec3 k1 = -current_value / time_constant;
    vec3 k2 = -(current_value + 0.5*elapsed_time*k1) / time_constant;
    vec3 k3 = -(current_value + 0.5*elapsed_time*k2) / time_constant;
    vec3 k4 = -(current_value + elapsed_time*k3) / time_constant;

    next_value = vec4(current_value + (elapsed_time / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4), 1.0);
}
