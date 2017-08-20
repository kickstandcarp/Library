#ifndef MATH_INTEGRATION_HPP
#define MATH_INTEGRATION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "math/arithmatic.hpp"

template <class T, class D> void    accumulate(T &v, const D &dv_dt, const float dt);
template <> void                    accumulate(glm::quat &v, const glm::vec3 &dv_dt, const float dt);

template <class T, class ...A> T    runge_kutta(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, A&&... args);



template <class T, class D>
void accumulate(T &v, const D &dv_dt, const float dt)
{
    v += dt*dv_dt;
}

template <class T, class ...A>
T runge_kutta(T (*dx_dt)(const float, const T&, A&&...), const T &x, const float t, const float dt, A&&... args)
{
    T k1 = dx_dt(t, x, std::forward<A>(args)...);
    T k2 = dx_dt(t + 0.5f*dt, x + 0.5f*dt*k1, std::forward<A>(args)...);
    T k3 = dx_dt(t + 0.5f*dt, x + 0.5f*dt*k2, std::forward<A>(args)...);
    T k4 = dx_dt(t + dt, x + dt*k3, std::forward<A>(args)...);

    return (1.0f / 6.0f)*(k1 + 2.0f*k2 + 2.0f*k3 + k4);
}

#endif
