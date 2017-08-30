#version 330 core

layout(location = 0) in vec3	vertex_position; // x, y, distance

out float                       scan_distance;

uniform	mat4x4			transform_matrix;



void main()
{
    scan_distance = vertex_position[2];

    gl_Position = transform_matrix*vec4(vertex_position.xy, -0.5, 1.0);
}
