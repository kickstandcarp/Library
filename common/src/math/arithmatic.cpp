#include <cmath>

#include "math/arithmatic.hpp"



template <>
int modulo<int>(const int dividend, const int divisor)
{
	int remainder = dividend % divisor;
	return remainder >= 0 ? remainder : remainder + divisor;
}

template <>
float modulo<float>(const float dividend, const float divisor)
{
	return dividend - std::floor(dividend / divisor)*divisor;
}
