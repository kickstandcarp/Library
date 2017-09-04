#version 330 core

layout(location = 0) out vec4   next_value;

in vec2                         position;

uniform float                   elapsed_time;
uniform sampler2D	        value_sampler;

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
}
