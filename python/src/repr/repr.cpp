#include <cmath>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>

#include "repr/repr.hpp"



template <>
std::string to_python_string<bool>(const bool value)
{
	return value ? "True" : "False";
}

template <>
std::string to_python_string<unsigned int>(const unsigned int value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}

template <>
std::string to_python_string<int>(const int value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}

template <>
std::string to_python_string<float>(const float value)
{
	if (std::isnan(value))
		return "nan";
	else if (std::isinf(value))
		return value > 0.0f ? "inf" : "-inf";

	int digits = static_cast<int>(std::floor(std::fabs(log(std::numeric_limits<float>::epsilon()) / std::log(10.0f))));

	std::stringstream stream;
	stream << std::showpoint << std::setprecision(digits) << (value == 0.0f ? 0.0f : value);
	std::string string = stream.str();

	unsigned int last_zero_index = std::min(string.size(), string.find('e'))-1;
	unsigned int first_zero_index = string.find_last_not_of('0', last_zero_index)+1;
	if (string[first_zero_index-1] == '.')
		first_zero_index++;

	if (first_zero_index <= last_zero_index)
		string.erase(first_zero_index, last_zero_index - first_zero_index+1);

	if (string.back() == '.')
		string.push_back('0');

	return string;
}
