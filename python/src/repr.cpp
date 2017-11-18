#include <cmath>
#include <limits>

#include "repr.hpp"



template <>
std::string to_python_string<bool>(const bool &value)
{
    return value ? "True" : "False";
}

template <>
std::string to_python_string<unsigned int>(const unsigned int &value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

template <>
std::string to_python_string<int>(const int &value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

template <>
std::string to_python_string<float>(const float &value)
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

template <>
std::string to_python_string<glm::bvec2>(const glm::bvec2 &value)
{
    std::stringstream stream;
    stream << "bvec2(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::bvec3 &value)
{
    std::stringstream stream;
    stream << "bvec3(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::bvec4 &value)
{
    std::stringstream stream;
    stream << "bvec4(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ", " << to_python_string(value.w) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::ivec2 &value)
{
    std::stringstream stream;
    stream << "ivec2(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::ivec3 &value)
{
    std::stringstream stream;
    stream << "ivec3(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::ivec4 &value)
{
    std::stringstream stream;
    stream << "ivec4(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ", " << to_python_string(value.w) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::vec2 &value)
{
    std::stringstream stream;
    stream << "vec2(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::vec3 &value)
{
    std::stringstream stream;
    stream << "vec3(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::vec4 &value)
{
    std::stringstream stream;
    stream << "vec4(" << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ", " << to_python_string(value.w) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::quat &value)
{
    std::stringstream stream;
    stream << "quat(" << to_python_string(value.w) << ", " << to_python_string(value.x) << ", " << to_python_string(value.y) << ", " << to_python_string(value.z) << ")";
    return stream.str();
}

template <>
std::string to_python_string(const glm::mat2x2 &value)
{
    std::stringstream stream;
    stream << "mat2x2([";
    mat_repr<2, 2>(value, stream);
    stream << "])";
    return stream.str();
}

template <>
std::string to_python_string(const glm::mat3x3 &value)
{
    std::stringstream stream;
    stream << "mat3x3([";
    mat_repr<3, 3>(value, stream);
    stream << "])";
    return stream.str();
}

template <>
std::string to_python_string(const glm::mat4x4 &value)
{
    std::stringstream stream;
    stream << "mat4x4([";
    mat_repr<4, 4>(value, stream);
    stream << "])";
    return stream.str();
}
