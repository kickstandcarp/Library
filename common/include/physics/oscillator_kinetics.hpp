#ifndef PHYSICS_OSCILLATOR_KINETICS_HPP
#define PHYSICS_OSCILLATOR_KINETICS_HPP

#include <memory>
#include <tuple>
#include <glm/gtc/constants.hpp>
#include "physics/kinetics.hpp"
#include "math/integration.hpp"
#include "geometry/path.hpp"
#include "geometry/path_vertex.hpp"

template <class T>
class OscillatorKinetics: public Kinetics<T>
{
	public:
		OscillatorKinetics(const T &value, const T &velocity, const T &external_acceleration, const T &frequency, const T &damping_ratio);
		virtual ~OscillatorKinetics();

		const std::shared_ptr<SegmentedPath<T> >	get_path();
		void										add_path(const float time);
		void										remove_path();

        T											steady_state_value_to_acceleration(const T &value) const;

        virtual void								step(const float elapsed_time, const float time);

        T											frequency, damping_ratio;
};

template <class T> T								oscillator_kinetics_d2x_dt2(const float dt, const T &dx_dt, const OscillatorKinetics<T> &kinetics);



template <class T>
OscillatorKinetics<T>::OscillatorKinetics(const T &value, const T &velocity, const T &external_acceleration, const T &frequency, const T &damping_ratio)
:   Kinetics<T>(std::make_tuple(value), std::make_tuple(velocity), std::make_tuple(external_acceleration)),
    frequency(frequency),
    damping_ratio(damping_ratio)
{

}

template <class T>
OscillatorKinetics<T>::~OscillatorKinetics()
{

}

template <class T>
const std::shared_ptr<SegmentedPath<T> > OscillatorKinetics<T>::get_path()
{
	return std::get<0>(this->value_paths);
}

template <class T>
void OscillatorKinetics<T>::add_path(const float time)
{
	this->Kinetics<T>::template add_path<0>(time);
}

template <class T>
void OscillatorKinetics<T>::remove_path()
{
	this->Kinetics<T>::template remove_path<0>();
}

template <class T>
T OscillatorKinetics<T>::steady_state_value_to_acceleration(const T &value) const
{
    return value*(glm::two_pi<float>()*glm::two_pi<float>()*frequency*frequency);
}

template <class T>
void OscillatorKinetics<T>::step(const float elapsed_time, const float time)
{
    T accelerations = runge_kutta<T, const OscillatorKinetics<T>&>(oscillator_kinetics_d2x_dt2<T>, std::get<0>(this->velocities), 0.0f, elapsed_time, *this);

    accumulate(std::get<0>(this->velocities), accelerations, elapsed_time);
    accumulate(std::get<0>(this->values), std::get<0>(this->velocities), elapsed_time);

	this->Kinetics<T>::template step_path<0>(elapsed_time, time);
}

template <class T>
T oscillator_kinetics_d2x_dt2(const float dt, const T &dx_dt, const OscillatorKinetics<T> &kinetics)
{
    T angular_frequency = glm::two_pi<T>()*kinetics.frequency;

	T x = std::get<0>(kinetics.values);
	accumulate(x, dx_dt, dt);

    return std::get<0>(kinetics.external_accelerations) - 2.0f*kinetics.damping_ratio*angular_frequency*dx_dt - angular_frequency*angular_frequency*x;
}

#endif
