#ifndef PHYSICS_INTEGRATION_HPP
#define PHYSICS_INTEGRATION_HPP

#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

const static std::vector<float>					runge_kutta_4_weights{0.0f, 1.0f / 2.0f, 1.0f / 2.0f, 1.0f};
const static std::vector<std::vector<float> >	runge_kutta_4_matrix{{}, {1.0f / 2.0f}, {0.0f, 1.0f / 2.0f}, {0.0f, 0.0f, 1.0f}};
const static std::vector<float>					runge_kutta_4_nodes{1.0f / 6.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 2.0f};

const static std::vector<float>					adaptive_runge_kutta_45_weights{0.0f, 1.0f/ 4.0f, 3.0f / 8.0f, 12.0f / 13.0f, 1.0f, 1.0f / 2.0f};
const static std::vector<std::vector<float> >	adaptive_runge_kutta_45_matrix{{}, {1.0f / 4.0f}, {3.0f / 32.0f, 9.0f / 32.0f}, {1932.0f / 2197.0f, -7200.0f / 2197.0f, 7296.0f / 2197.0f}, {439.0f / 216.0f, -8.0f, 3680.0f / 513.0f, -845.0f / 4104.0f}, {-8.0f / 27.0f, 2.0f, -3544.0f / 2565.0f, 1859.0f / 4104.0f, -11.0f / 40.0f}};
const static std::vector<std::vector<float> >	adaptive_runge_kutta_45_nodes{{16.0f / 135.0f, 0.0f, 6656.0f / 12825.0f, 28561.0f / 56430.0f, -9.0f / 50.0f, 2.0f / 55.0f}, {25.0f / 216.0f, 0.0f, 1408.0f / 2565.0f, 2197.0f / 4104.0f, -1.0f / 5.0f, 0.0f}};

template <class T> T							ingetrate(const float dt, const T &x, const T &dx_dt);
template <> glm::quat							ingetrate(const float dt, const glm::quat &x, const glm::quat &dx_dt);



template <class T>
T ingetrate(const float dt, const T &x, const T &dx_dt)
{
    return x + dt*dx_dt;
}

#endif
