#include <stdexcept>

#include "math/arithmatic.hpp"



template <>
float length<bool>(const bool &a, const bool &b)
{
	throw std::runtime_error("not implemented yet");
}

template <>
float length<glm::quat>(const glm::quat &a, const glm::quat &b)
{
	throw std::runtime_error("not implemented yet");
}

template <>
float distance<bool>(const bool &a, const bool &b)
{
	throw std::runtime_error("not implemented yet");
}

template <>
float distance<glm::quat>(const glm::quat &a, const glm::quat &b)
{
	throw std::runtime_error("not implemented yet");
}
