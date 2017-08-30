#version 330 core

layout(location = 0) out vec4	fragment_color;

// in vec2				            texture_position;

uniform vec4					color;
// uniform sampler2D				texture_sampler;



void main()
{
	fragment_color = color; // + texture(texture_sampler, texture_position);
}
