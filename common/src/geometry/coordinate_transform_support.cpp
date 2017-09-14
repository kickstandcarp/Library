#include "geometry/coordinate_transform_support.hpp"



glm::vec3 translation(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to)
{
	std::shared_ptr<CoordinateTransform> coordinate_transform;
	glm::vec3 from_translation(0.0f), to_translation(0.0f);

	coordinate_transform = from;
	while (coordinate_transform)
	{
		from_translation += coordinate_transform->get_translation();
		coordinate_transform = coordinate_transform->get_parent();
	}

	coordinate_transform = to;
	while (coordinate_transform)
	{
		to_translation += coordinate_transform->get_translation();
		coordinate_transform = coordinate_transform->get_parent();
	}

	return from_translation - to_translation;
}

glm::quat rotation(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to)
{
	std::shared_ptr<CoordinateTransform> coordinate_transform;
	glm::quat from_rotation(1.0f, 0.0f, 0.0f, 0.0f), to_rotation(1.0f, 0.0f, 0.0f, 0.0f);

	coordinate_transform = from;
	while (coordinate_transform)
	{
		from_rotation *= coordinate_transform->get_rotation();
		coordinate_transform = coordinate_transform->get_parent();
	}

	coordinate_transform = to;
	while (coordinate_transform)
	{
		to_rotation *= coordinate_transform->get_rotation();
		coordinate_transform = coordinate_transform->get_parent();
	}

	return glm::normalize(from_rotation*glm::conjugate(to_rotation));
}

glm::vec3 scaling(const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to)
{
	std::shared_ptr<CoordinateTransform> coordinate_transform;
	glm::vec3 from_scaling(1.0f), to_scaling(1.0f);

	coordinate_transform = from;
	while (coordinate_transform)
	{
		from_scaling *= coordinate_transform->get_scaling();
		coordinate_transform = coordinate_transform->get_parent();
	}

	coordinate_transform = to;
	while (coordinate_transform)
	{
		to_scaling *= coordinate_transform->get_scaling();
		coordinate_transform = coordinate_transform->get_parent();
	}

	return from_scaling / to_scaling;
}

glm::vec3 transform_position(const glm::vec3 &position, const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to)
{
	glm::vec3 global_position = from ? from->transform_position(position, true) : position;
	return to ? to->transform_position(global_position, false) : global_position;
}

glm::vec3 transform_direction(const glm::vec3 &direction, const std::shared_ptr<CoordinateTransform> &from, const std::shared_ptr<CoordinateTransform> &to)
{
	glm::vec3 global_direction = from ? from->transform_direction(direction, true) : direction;
	return to ? to->transform_direction(global_direction, false) : global_direction;
}
