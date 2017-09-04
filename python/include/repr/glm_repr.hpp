#ifndef REPR_GLM_REPR_HPP
#define REPR_GLM_REPR_HPP

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "repr/repr.hpp"

std::string												bvec2_repr(const glm::bvec2 &vec);
std::string												bvec3_repr(const glm::bvec3 &vec);
std::string												bvec4_repr(const glm::bvec4 &vec);

std::string												ivec2_repr(const glm::ivec2 &vec);
std::string												ivec3_repr(const glm::ivec3 &vec);
std::string												ivec4_repr(const glm::ivec4 &vec);

std::string												vec2_repr(const glm::vec2 &vec);
std::string												vec3_repr(const glm::vec3 &vec);
std::string												vec4_repr(const glm::vec4 &vec);

std::string												quat_repr(const glm::quat &quat);

std::string												mat2x2_repr(const glm::mat2x2 &mat);
std::string												mat3x3_repr(const glm::mat3x3 &mat);
std::string												mat4x4_repr(const glm::mat4x4 &mat);

template <unsigned int R, unsigned int C, class T> void	mat_repr(const T &mat, std::stringstream &stream);



template <unsigned int R, unsigned int C, class T>
void mat_repr(const T &mat, std::stringstream &stream)
{
	unsigned int field_length = 0;
	std::vector<std::string> strings(R*C);

	const float* value_ptr = glm::value_ptr(mat);
	for (unsigned int index = 0;  index < R*C; index++)
	{
		strings[index] = to_python_string(*(value_ptr + index));
		field_length = std::max(field_length, static_cast<unsigned int>(strings[index].size()));
	}

	for (unsigned int row = 0; row < R; row++)
	{
		if (row > 0)
			stream << "        [";
		else
			stream << "[";
		for (unsigned int column = 0; column < C; column++)
		{
			stream << std::left << std::setw(field_length) << strings[R*column + row];
			if (column < C-1)
				stream << ", ";
		}
		stream << "]";
		if (row < R-1)
			stream << "," << std::endl;
	}
}

#endif
