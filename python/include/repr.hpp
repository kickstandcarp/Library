#ifndef PYTHON_REPR_HPP
#define PYTHON_REPR_HPP

#include <iomanip>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template <class T> std::string                          to_python_string(const T &value);

template <unsigned int R, unsigned int C, class T> void mat_repr(const T &mat, std::stringstream &stream);



template <unsigned int R, unsigned int C, class T>
void mat_repr(const T &mat, std::stringstream &stream)
{
    unsigned int field_length = 0;
    std::vector<std::string> strings(R*C);

    const float *value_ptr = glm::value_ptr(mat);
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
