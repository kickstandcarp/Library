#version 330 core

layout(location = 0) out vec4   fragment_color;

uniform int                     size;
uniform float                   width, intensity;

uniform sampler2D	        color_sampler;



void main()
{
    vec4 color = vec4(0.0);
    float normalization = 0.0;

    ivec2 texture_position = ivec2(gl_FragCoord.xy);
    for (int i = 0; i < size; i++)
    {
        int offset = i - (size / 2);
        float coefficient = exp(-float(offset*offset) / (2.0*width*width));

        vec4 texture_color = texelFetch(color_sampler, texture_position + ivec2(0, offset), 0);
        color += coefficient*texture_color;

        normalization += coefficient;
    }

    fragment_color = intensity*color / normalization;
}
