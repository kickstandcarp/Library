#version 330 core

layout(location = 0) out vec4   forcing_value;

in float                        distance;

uniform float                   elapsed_time, elapsed_distance;
uniform float                   beam_width, beam_velocity, beam_intensity;

float                           beam_integral(float width, float d_min, float d_max);
float                           erf(float x);



void main()
{
    float value = beam_intensity*beam_integral(beam_width, distance - elapsed_distance - elapsed_time*beam_velocity, distance - elapsed_distance)*elapsed_time;

    forcing_value = vec4(vec3(value), 1.0);
}

float beam_integral(float width, float d_min, float d_max)
{
    return 1.253314137*width*(erf(d_max / (1.414213562*width)) - erf(d_min / (1.414213562*width)));
}

float erf(float x)
{
    float t = 1.0 / (1.0 + 0.3275911*abs(x));
    return sign(x)*(1.0 - (0.254829592*t - 0.284496736*t*t + 1.421413741*t*t*t - 1.453152027*t*t*t*t + 1.061405429*t*t*t*t*t)*exp(-(x*x)));
}
