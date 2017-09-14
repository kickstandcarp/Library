#version 330 core

layout(location = 0) out vec4   fragment_color;

uniform int                     size;
uniform float                   width;

uniform sampler2D	        intermediate_color_sampler, source_color_sampler;



void main()
{
    vec4 color = vec4(0.0);
    float normalization = 0.0;

    ivec2 texture_position = ivec2(gl_FragCoord.xy);
    for (int i = 0; i < size; i++)
    {
        int offset = i - (size / 2);
        float coefficient = exp(-float(offset*offset) / (2.0*width*width));

        vec4 texture_color = texelFetch(intermediate_color_sampler, texture_position + ivec2(offset, 0), 0);
        color += coefficient*texture_color;

        normalization += coefficient;
    }

    fragment_color = (color / normalization) + texelFetch(source_color_sampler, texture_position, 0);
}
