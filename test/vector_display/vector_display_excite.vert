#version 330 core

layout(location = 0) in vec3	vertex_position; // x, y, scan_distance

out float                       vertex_scan_distance;

uniform	mat4x4			transform_matrix;



void main()
{
    vertex_scan_distance = vertex_position[2];

    gl_Position = transform_matrix*vec4(vertex_position.xy, 0.0, 1.0);
    gl_Position.z = 0.0;
}
