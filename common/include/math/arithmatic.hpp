#ifndef MATH_ARITHMATIC_HPP
#define MATH_ARITHMATIC_HPP

#include <array>
#include <glm/gtc/constants.hpp>

template <class T> T                                modulo(const T a, const T b);
template <class T> T                                angle_difference(const T a, const T b);

template <class T, std::size_t D> std::array<T, D>  operator+(const std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>& operator+=(std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>  operator*(const std::array<T, D> &a, const std::array<T, D> &b);
template <class T, std::size_t D> std::array<T, D>  operator*(const T &a, const std::array<T, D> &b);



template <class T>
T angle_difference(const T angle_1, const T angle_2)
{
    return modulo<T>(angle_1 - angle_2 + glm::pi<T>(), static_cast<T>(2)*glm::pi<T>()) - glm::pi<T>();
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

#endif
