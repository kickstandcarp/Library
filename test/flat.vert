#version 330 core

layout(location = 0) in vec3	vertex_position;
// layout(location = 1) in vec2	vertex_texture_position;

// out vec2                        texture_position;

uniform mat4x4					transform_matrix;



void main()
{
	// texture_position = vertex_texture_position;
    gl_Position = transform_matrix*vec4(vertex_position, 1.0);
}
