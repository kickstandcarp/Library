#include <glm/gtc/constants.hpp>

#include "physics/oscillator_kinetics.hpp"
#include "math/integration.hpp"



OscillatorKinetics::OscillatorKinetics(const float value, const float velocity, const float frequency, const float damping_ratio, const float acceleration)
:   frequency(frequency),
    damping_ratio(damping_ratio),
    acceleration(acceleration),
    value(value),
    velocity(velocity)
{

}

OscillatorKinetics::~OscillatorKinetics()
{

}

float OscillatorKinetics::steady_state_value_acceleration(const float value) const
{
    return value*(glm::two_pi<float>()*glm::two_pi<float>()*frequency*frequency);
}

void OscillatorKinetics::step(const float elapsed_time)
{
    float acceleration = runge_kutta<float, const OscillatorKinetics&>(oscillator_kinetics_d2x_dt2, this->velocity, 0.0f, elapsed_time, *this);

    accumulate(this->velocity, acceleration, elapsed_time);
    accumulate(this->value, this->velocity, elapsed_time);
}

float oscillator_kinetics_d2x_dt2(const float dt, const float &dx_dt, const OscillatorKinetics &kinetics)
{
    float angular_frequency = glm::two_pi<float>()*kinetics.frequency;
    return kinetics.acceleration - 2.0f*kinetics.damping_ratio*angular_frequency*dx_dt - angular_frequency*angular_frequency*kinetics.value;
}
