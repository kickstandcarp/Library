#include <sstream>

#include "repr/repr.hpp"
#include "repr/coordinate_repr.hpp"



std::string	coordinate_transform_repr(const CoordinateTransform &coordinate_transform)
{
	std::stringstream stream;
	stream << "CoordinateTransform(";
	stream << "translation=(" << to_python_string(coordinate_transform.get_translation().x) << ", " << to_python_string(coordinate_transform.get_translation().y) << ", " << to_python_string(coordinate_transform.get_translation().z) << "), ";
	stream << "rotation=(" << to_python_string(coordinate_transform.get_rotation().w) << ", " << to_python_string(coordinate_transform.get_rotation().x) << ", " << to_python_string(coordinate_transform.get_rotation().y) << ", " << to_python_string(coordinate_transform.get_rotation().z) << "), ";
	stream << "scaling=(" << to_python_string(coordinate_transform.get_scaling().x) << ", " << to_python_string(coordinate_transform.get_scaling().y) << ", " << to_python_string(coordinate_transform.get_scaling().z) << "), ";
	stream << "origin_translation=(" << to_python_string(coordinate_transform.get_origin_translation().x) << ", " << to_python_string(coordinate_transform.get_origin_translation().y) << ", " << to_python_string(coordinate_transform.get_origin_translation().z) << "))";
	return stream.str();
}

std::string camera_repr(const Camera &camera)
{
	std::stringstream stream;
	stream << "Camera(";
	stream << "coordinate_transform=" << coordinate_transform_repr(const_cast<Camera*>(&camera)->get_coordinate_transform()) << ", ";
	stream << "width=" << to_python_string(camera.get_width()) << ", ";
	stream << "height=" << to_python_string(camera.get_height()) << ", ";
	stream << "near_z=" << to_python_string(camera.get_near_z()) << ", ";
	stream << "far_z=" << to_python_string(camera.get_far_z()) << ", ";
	stream << "orhtographic=" << to_python_string(camera.get_orthographic()) << ")";
	return stream.str();
}
