#ifndef PHYSICS_OSCILLATOR_KINETICS_HPP
#define PHYSICS_OSCILLATOR_KINETICS_HPP

#include "math/integration.hpp"

class OscillatorKinetics
{
	public:
		OscillatorKinetics(const float value, const float velocity, const float frequency, const float damping_ratio, const float acceleration);
		~OscillatorKinetics();

        float   steady_state_value_acceleration(const float value) const;

        void	step(const float elapsed_time);

        float   frequency, damping_ratio, acceleration;

        float   value;
        float   velocity;
};

float           oscillator_kinetics_d2x_dt2(const float dt, const float &dx_dt, const OscillatorKinetics &kinetics);

#endif
