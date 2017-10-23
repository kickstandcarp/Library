#ifndef MATH_ARITHMATIC_HPP
#define MATH_ARITHMATIC_HPP

#include <cmath>
#include <array>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

template <class T> T                                modulof(const T &a, const T &b);
template <class T> T								moduloi(const T &a, const T &b);

template <class T> T                                angle_difference(const T &a, const T &b);

template <class T> float							length(const T &a, const T &b);
template <> float									length(const bool &a, const bool &b);
template <> float									length(const glm::quat &a, const glm::quat &b);
template <class T> float							distance(const T &a, const T &b);
template <> float									distance(const bool &a, const bool &b);
template <> float									distance(const glm::quat &a, const glm::quat &b);

template <class T, std::size_t D> std::array<T, D>  operator+(const std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>& operator+=(std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>  operator*(const std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>  operator*(const T &a, const std::array<T, D> &b);



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

template <class T, std::size_t D>
std::array<T, D> operator+(const std::array<T, D> &a, const std::array<T, D> &b)
{
    std::array<T, D> c;
    for (unsigned int i = 0; i < D; i++)
        c[i] = a[i] + b[i];
    return c;
}

template <class T, std::size_t D>
std::array<T, D>& operator+=(std::array<T, D> &a, const std::array<T, D> &b)
{
    for (unsigned int i = 0; i < D; i++)
        a[i] += b[i];
    return a;
}

template <class T, std::size_t D>
std::array<T, D> operator*(const std::array<T, D> &a, const std::array<T, D> &b)
{
    std::array<T, D> c;
    for (unsigned int i = 0; i < D; i++)
        c[i] = a[i]*b[i];
    return c;
}

template <class T, std::size_t D>
std::array<T, D> operator*(const T &a, const std::array<T, D> &b)
{
    std::array<T, D> c;
    for (unsigned int i = 0; i < D; i++)
        c[i] = a*b[i];
    return c;
}

template <class T>
float length(const T &a, const T &b)
{
	return glm::length(a, b);
}

template <class T>
float distance(const T &a, const T &b)
{
	return glm::distance(a, b);
}

#endif
