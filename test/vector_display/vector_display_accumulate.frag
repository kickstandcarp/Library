#version 330 core

layout(location = 0) out vec4   next_value;

in vec2                         texture_position;

uniform float                   elapsed_time;
uniform float                   excitation_time_constant, decay_time_constant;

uniform sampler2D	        forcing_value_texture, current_value_texture;



void main()
{
    vec4 forcing_value = texture(forcing_value_texture, texture_position);
    vec4 current_value = texture(current_value_texture, texture_position);

    vec4 velocity = (forcing_value - current_value) / mix(vec4(excitation_time_constant), vec4(decay_time_constant), lessThan(forcing_value, current_value));

    next_value = current_value + velocity*elapsed_time;
}
