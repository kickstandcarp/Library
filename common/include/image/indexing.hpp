#ifndef IMAGE_INDEXING_HPP
#define IMAGE_INDEXING_HPP

#include <array>

template <std::size_t D> unsigned int                   ravel_index(const std::array<unsigned int, D> &indices, const std::array<unsigned int, D> &shape);
template <std::size_t D> std::array<unsigned int, D>    unravel_index(const unsigned int index, const std::array<unsigned int, D> &shape);



template <std::size_t D>
unsigned int ravel_index(const std::array<unsigned int, D> &indices, const std::array<unsigned int, D> &shape)
{
    unsigned int dim = 1, raveled_index = 0;
    for (unsigned int i = 0; i < D; i++)
    {
        raveled_index += dim*indices[D-1-i];
        dim *= shape[D-1-i];
    }

    return raveled_index;
}

template <std::size_t D>
std::array<unsigned int, D> unravel_index(const unsigned int index, const std::array<unsigned int, D> &shape)
{
    unsigned int dim = 1;
    std::array<unsigned int, D> unraveled_index;
    for (unsigned int i = 0; i < D; i++)
    {
        unraveled_index[D-1-i] = (index / dim) % shape[D-1-i];
        dim *= shape[D-1-i];
    }

    return unraveled_index;
}

#endif
