#ifndef MATH_INTEGRATION_HPP
#define MATH_INTEGRATION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "math/arithmatic.hpp"

template <class T> void								accumulate(T &v, const T &dv_dt, const float dt);
template <> void									accumulate(glm::quat &v, const glm::quat &dv_dt, const float dt);

template <class T, class ...A> T					runge_kutta(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, A&&... args);
template <class T, class ...A> std::tuple<T, float>	runge_kutta_fehlberg(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, float (*error)(const T&, const T&), const float tolerance, const float min_dt, A&&... args);



template <class T>
void accumulate(T &v, const T &dv_dt, const float dt)
{
    v += dt*dv_dt;
}

template <class T, class ...A>
T runge_kutta(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, A&&... args)
{
	T k1 = dx_dt(t, x, std::forward<A>(args)...);
    T k2 = dx_dt(t + 0.5f*dt, x + dt*0.5f*k1, std::forward<A>(args)...);
    T k3 = dx_dt(t + 0.5f*dt, x + dt*0.5f*k2, std::forward<A>(args)...);
    T k4 = dx_dt(t + dt, x + dt*k3, std::forward<A>(args)...);

    return (1.0f / 6.0f)*(k1 + 2.0f*k2 + 2.0f*k3 + k4);
}

template <class T, class ...A>
std::tuple<T, float> runge_kutta_fehlberg(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, float (*error)(const T&, const T&), const float tolerance, const float min_dt, A&&... args)
{
	float actual_dt = dt;
	while (true)
	{
		T k1 = dx_dt(t, x, std::forward<A>(args)...);
		T k2 = dx_dt(t + 0.25f*actual_dt, x + actual_dt*0.25f*k1, std::forward<A>(args)...);
		T k3 = dx_dt(t + 0.375f*actual_dt, x + actual_dt*(0.09375f*k1 + 0.28125f*k2), std::forward<A>(args)...);
		T k4 = dx_dt(t + 0.9230769230769231f*actual_dt, x + actual_dt*(0.8793809740555303f*k1 - 3.277196176604461f*k2 + 3.3208921256258535f*k3), std::forward<A>(args)...);
		T k5 = dx_dt(t + actual_dt, x + actual_dt*(2.0324074074074074f*k1 - 8.0f*k2 + 7.173489278752436f*k3 - 0.20589668615984405f*k4), std::forward<A>(args)...);
		T k6 = dx_dt(t + 0.5f*actual_dt, x + actual_dt*(-0.2962962962962963f*k1 + 2.0f*k2 - 1.3816764132553607f*k3 + 0.4529727095516569f*k4 - 0.275f*k5), std::forward<A>(args)...);
		
		T next_x = 0.11851851851851852f*k1 + 0.5189863547758284f*k3 + 0.5061314903420167f*k4 - 0.18f*k5 + 0.03636363636363636f*k6;

		float error_estimate = error(next_x, 0.11574074074074074f*k1 + 0.5489278752436647f*k3 + 0.5353313840155945f*k4 - 0.2f*k5);
		if (error_estimate <= tolerance || actual_dt <= min_dt)
			return std::make_tuple(next_x, actual_dt);
		else
		{
			actual_dt *= 0.9f*std::min(10.0f, std::max(0.1f, std::pow(tolerance / error_estimate, 0.2f)));
			actual_dt = std::max(min_dt, actual_dt);
		}
	}
}

#endif
