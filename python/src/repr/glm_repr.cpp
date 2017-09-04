#include "repr/glm_repr.hpp"



std::string bvec2_repr(const glm::bvec2 &vec)
{
	std::stringstream stream;
	stream << "bvec2(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ")";
	return stream.str();
}

std::string bvec3_repr(const glm::bvec3 &vec)
{
	std::stringstream stream;
	stream << "bvec3(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ")";
	return stream.str();
}

std::string bvec4_repr(const glm::bvec4 &vec)
{
	std::stringstream stream;
	stream << "bvec4(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ", " << to_python_string(vec.w) << ")";
	return stream.str();
}

std::string ivec2_repr(const glm::ivec2 &vec)
{
	std::stringstream stream;
	stream << "ivec2(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ")";
	return stream.str();
}

std::string ivec3_repr(const glm::ivec3 &vec)
{
	std::stringstream stream;
	stream << "ivec3(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ")";
	return stream.str();
}

std::string ivec4_repr(const glm::ivec4 &vec)
{
	std::stringstream stream;
	stream << "ivec4(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ", " << to_python_string(vec.w) << ")";
	return stream.str();
}

std::string vec2_repr(const glm::vec2 &vec)
{
    std::stringstream stream;
	stream << "vec2(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ")";
    return stream.str();
}

std::string vec3_repr(const glm::vec3 &vec)
{
    std::stringstream stream;
	stream << "vec3(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ")";
    return stream.str();
}

std::string vec4_repr(const glm::vec4 &vec)
{
	std::stringstream stream;
	stream << "vec4(" << to_python_string(vec.x) << ", " << to_python_string(vec.y) << ", " << to_python_string(vec.z) << ", " << to_python_string(vec.w) << ")";
	return stream.str();
}

std::string quat_repr(const glm::quat &quat)
{
	std::stringstream stream;
	stream << "quat(" << to_python_string(quat.w) << ", " << to_python_string(quat.x) << ", " << to_python_string(quat.y) << ", " << to_python_string(quat.z) << ")";
	return stream.str();
}

std::string mat2x2_repr(const glm::mat2x2 &mat)
{
	std::stringstream stream;
	stream << "mat2x2([";
	mat_repr<2, 2>(mat, stream);
	stream << "])";
	return stream.str();
}

std::string mat3x3_repr(const glm::mat3x3 &mat)
{
	std::stringstream stream;
	stream << "mat3x3([";
	mat_repr<3, 3>(mat, stream);
	stream << "])";
	return stream.str();
}

std::string mat4x4_repr(const glm::mat4x4 &mat)
{
	std::stringstream stream;
	stream << "mat4x4([";
	mat_repr<4, 4>(mat, stream);
	stream << "])";
	return stream.str();
}
