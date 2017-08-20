#ifndef REPR_COORDINATE_REPR_HPP
#define REPR_COORDINATE_REPR_HPP

#include <string>
#include "coordinate/coordinate_transform.hpp"
#include "coordinate/camera.hpp"

std::string	coordinate_transform_repr(const CoordinateTransform &coordinate_transform);
std::string	camera_repr(const Camera &camera);

#endif
