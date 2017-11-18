#ifndef PHYSICS_OSCILLATOR_KINETICS_HPP
#define PHYSICS_OSCILLATOR_KINETICS_HPP

#include <memory>
#include <tuple>
#include <glm/gtc/constants.hpp>
#include "clock.hpp"
#include "physics/kinetics.hpp"
#include "physics/integration.hpp"
#include "parametric/segment_curve.hpp"

template <class T>
class OscillatorKinetics: public Kinetics<T>
{
    public:
        OscillatorKinetics(const T &value, const T &velocity, const T &frequency, const T &damping_ratio);
        virtual ~OscillatorKinetics();

        const std::shared_ptr<const SegmentCurve<T> >   get_history() const;
        void                                            add_history(const float time);
        void                                            remove_history();

        T                                               steady_state_value_to_acceleration(const T &value) const;

        virtual void                                    step(const Clock &clock);

        T                                               frequency, damping_ratio;

	private:
		virtual std::tuple<T>							runge_kutta_midpoint(const float dt, const std::tuple<T> &x, const std::vector<float> &coefficients, const std::vector<std::tuple<T> > &dx_dts, const bool derivative) const;
		virtual float									adaptive_runge_kutta_relative_error(const std::tuple<T> &dx_dt_1, const std::tuple<T> &dx_dt_2) const;
		virtual std::tuple<T>							d2x_dt2(const float t, const std::tuple<T> &x, const std::tuple<T> &dx_dt) const;
};



template <class T>
OscillatorKinetics<T>::OscillatorKinetics(const T &value, const T &velocity, const T &frequency, const T &damping_ratio)
:   Kinetics<T>(std::make_tuple(value), std::make_tuple(velocity)),
    frequency(frequency),
    damping_ratio(damping_ratio)
{
	this->external_accelerations = std::make_tuple(T(0));
}

template <class T>
OscillatorKinetics<T>::~OscillatorKinetics()
{

}

template <class T>
const std::shared_ptr<const SegmentCurve<T> > OscillatorKinetics<T>::get_history() const
{
    return std::get<0>(this->value_histories);
}

template <class T>
void OscillatorKinetics<T>::add_history(const float time)
{
    this->Kinetics<T>::template add_history<0>(time, CurveInterpolation::cubic);
}

template <class T>
void OscillatorKinetics<T>::remove_history()
{
    this->Kinetics<T>::template remove_history<0>();
}

template <class T>
T OscillatorKinetics<T>::steady_state_value_to_acceleration(const T &value) const
{
    return value*(glm::two_pi<float>()*glm::two_pi<float>()*frequency*frequency);
}

template <class T>
void OscillatorKinetics<T>::step(const Clock &clock)
{
	// this->Kinetics<T>::runge_kutta_4(clock);
	this->Kinetics<T>::adaptive_runge_kutta_45(clock);
    this->Kinetics<T>::template step_history<0>(clock);
}

template <class T>
std::tuple<T> OscillatorKinetics<T>::runge_kutta_midpoint(const float dt, const std::tuple<T> &x, const std::vector<float> &coefficients, const std::vector<std::tuple<T> > &dx_dts, const bool derivative) const
{
	T dx_dt(0);
	for (unsigned int i = 0; i < coefficients.size(); i++)
		dx_dt += coefficients[i]*std::get<0>(dx_dts[i]);

	T midpoint = ingetrate(dt, std::get<0>(x), dx_dt);

	return std::tuple<T>(midpoint);
}

template <class T>
float OscillatorKinetics<T>::adaptive_runge_kutta_relative_error(const std::tuple<T> &dx_dt_1, const std::tuple<T> &dx_dt_2) const
{
	float dx_dt_error = glm::distance(std::get<0>(dx_dt_1), std::get<0>(dx_dt_2));
	float dx_dt_magnitude = 0.5f*(glm::length(std::get<0>(dx_dt_1)) + glm::length(std::get<0>(dx_dt_2)));

	return dx_dt_magnitude == 0.0f ? 0.0f : dx_dt_error / dx_dt_magnitude;
}

template <class T>
std::tuple<T> OscillatorKinetics<T>::d2x_dt2(const float t, const std::tuple<T> &x, const std::tuple<T> &dx_dt) const
{
	T angular_frequency = glm::two_pi<T>()*this->frequency;
	T d2x_dt2 = std::get<0>(this->external_accelerations) - 2.0f*this->damping_ratio*angular_frequency*std::get<0>(dx_dt) - angular_frequency*angular_frequency*std::get<0>(x);

	return std::tuple<T>(d2x_dt2);
}

#endif
