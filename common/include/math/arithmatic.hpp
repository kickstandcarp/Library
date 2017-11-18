#ifndef MATH_ARITHMATIC_HPP
#define MATH_ARITHMATIC_HPP

#include <cmath>
#include <array>
#include <glm/gtc/constants.hpp>

template <class T> T                                modulof(const T &a, const T &b);
template <class T> T								moduloi(const T &a, const T &b);

template <class T> T                                angle_difference(const T &a, const T &b);



template <class T>
T modulof(const float &a, const float &b)
{
    return a - glm::floor(a / b)*a;
}

template <class T>
T moduloi(const T &a, const T &b)
{
    T r = a % b;
    return glm::mix(r + b, r, glm::greaterThanEqual(r, T(0)));
}

template <class T>
T angle_difference(const T &angle_1, const T &angle_2)
{
    return modulof<T>(angle_1 - angle_2 + glm::pi<T>(), glm::two_pi<T>()) - glm::pi<T>();
}

#endif
