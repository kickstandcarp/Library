#ifndef COORDINATE_COORDINATE_TRANSFORM_SUPPORT_HPP
#define COORDINATE_COORDINATE_TRANSFORM_SUPPORT_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "coordinate_transform.hpp"

glm::vec3	translation(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to);
glm::quat	rotation(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to);
glm::vec3	scaling(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to);

glm::vec3	transform_position(const glm::vec3 &position, const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to);
glm::vec3	transform_direction(const glm::vec3 &direction, const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to);

#endif
