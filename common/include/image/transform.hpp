#ifndef IMAGE_TRANSFORM_HPP
#define IMAGE_TRANSFORM_HPP

#include <utility>
#include <vector>
#include <array>
#include "image/indexing.hpp"

template <class T> void flip_vertical_inplace(std::vector<T> &data, const std::array<unsigned int, 3> &shape);



template <class T> 
void flip_vertical_inplace(std::vector<T> &data, const std::array<unsigned int, 3> &shape)
{
    std::array<unsigned int, 3> indices;
    for (indices[0] = 0; indices[0] < shape[0] / 2; indices[0]++)
    {
        for (indices[1] = 0; indices[1] < shape[1]; indices[1]++)
        {
            for (indices[2] = 0; indices[2] < shape[2]; indices[2]++)
            {
                unsigned int index = ravel_index(indices, shape);
                unsigned int flipped_index = ravel_index(std::array<unsigned int, 3>{{shape[0]-1 - indices[0], indices[1], indices[2]}}, shape);
                std::swap(data[index], data[flipped_index]);
            }
        }
    }
}

#endif
