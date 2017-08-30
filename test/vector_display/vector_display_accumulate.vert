#version 330 core

layout(location = 0) in vec2	vertex_position;

out vec2                        texture_position;


void main()
{
    texture_position = 0.5*(vertex_position + 1.0);

    gl_Position = vec4(vertex_position, 0.5, 1.0);
}
