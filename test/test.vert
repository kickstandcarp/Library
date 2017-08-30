#version 330 core

layout(shared) uniform;

layout(location = 0) in vec3	data;

out vec3                        transformed_data_1;
out vec3                        transformed_data_2;

uniform uniform_buffer
{
    float                       uniform_buffer_float_1;
    float			uniform_buffer_float_2;
    vec3[2]                     uniform_buffer_vec3;
};

uniform vec3			uniform_vec3;
uniform ivec2[3]                uniform_ivec2;



void main()
{
	transformed_data_1 = data + uniform_vec3 + vec3(uniform_buffer_float_1) + vec3(uniform_ivec2[0], 0.0) + vec3(uniform_ivec2[2], 0.0);
	transformed_data_2 = data - uniform_vec3 + uniform_buffer_vec3[0] + vec3(uniform_ivec2[1], 0.0);
}
