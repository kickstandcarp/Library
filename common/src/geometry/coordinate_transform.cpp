#include <stdexcept>
#include <vector>

#include <glm/gtx/transform.hpp>

#include "geometry/coordinate_transform.hpp"



CoordinateTransform::CoordinateTransform(const glm::vec3 &translation, const glm::quat &rotation, const glm::vec3 &scaling, const glm::vec3 &origin_translation)
:	translation(translation),
	scaling(scaling),
	origin_translation(origin_translation),
	rotation(glm::quat(rotation)),
	parent(std::shared_ptr<CoordinateTransform>()),
	is_updated(false)
{

}

CoordinateTransform::~CoordinateTransform()
{
	for (auto const &child : this->children)
	{
		if (!child.expired())
			child.lock()->set_parent(this->parent);
	}
}

glm::vec3 CoordinateTransform::get_translation() const
{
	return this->translation;
}

glm::quat CoordinateTransform::get_rotation() const
{
	return this->rotation;
}

glm::vec3 CoordinateTransform::get_scaling() const
{
	return this->scaling;
}

glm::vec3 CoordinateTransform::get_origin_translation() const
{
	return this->origin_translation;
}

const std::shared_ptr<CoordinateTransform> CoordinateTransform::get_parent() const
{
	return this->parent;
}

const glm::mat4x4& CoordinateTransform::get_local_matrix()
{
	this->set_matrices();
	return this->local_matrix;
}

const glm::mat4x4& CoordinateTransform::get_global_matrix()
{
	this->set_matrices();
	return this->global_matrix;
}

const glm::mat4x4& CoordinateTransform::get_inverse_global_matrix()
{
	this->set_matrices();
	return this->inverse_global_matrix;
}

bool CoordinateTransform::get_updated() const
{
	return this->is_updated;
}

void CoordinateTransform::set_translation(const glm::vec3 &translation)
{
	this->translation = translation;
	this->set_updated(false);
}

void CoordinateTransform::set_rotation(const glm::quat &rotation)
{
	this->rotation = rotation;
	this->set_updated(false);
}

void CoordinateTransform::set_scaling(const glm::vec3 &scaling)
{
	this->scaling = scaling;
	this->set_updated(false);
}

void CoordinateTransform::set_origin_translation(const glm::vec3 &origin_translation)
{
	this->origin_translation = origin_translation;
	this->set_updated(false);
}

void CoordinateTransform::set_parent(const std::shared_ptr<CoordinateTransform> &coordinate_transform)
{
	if (this->parent.get() == coordinate_transform.get())
		return;

	if (this == coordinate_transform.get())
		throw std::invalid_argument("invalid circular reference");

	std::vector<const CoordinateTransform*> parents{this, coordinate_transform.get()}; // Check for circular references
	while (parents.back() != nullptr)
	{
		const CoordinateTransform* parent = parents.back()->get_parent().get();
		if (std::find(parents.begin(), parents.end(), parent) != parents.end())
			throw std::invalid_argument("invalid circular reference");
		else
			parents.push_back(parent);
	}

	if (this->parent)
		this->parent->children.erase(this->std::enable_shared_from_this<CoordinateTransform>::shared_from_this());
	if (coordinate_transform)
		coordinate_transform->children.insert(this->std::enable_shared_from_this<CoordinateTransform>::shared_from_this());
	this->parent = coordinate_transform;

	this->set_updated(false);
}

void CoordinateTransform::translate(const glm::vec3 &translation)
{
	this->set_translation(this->translation + translation);
}

void CoordinateTransform::rotate(const glm::quat &rotation)
{
	this->set_rotation(this->rotation*rotation);
}

void CoordinateTransform::scale(const glm::vec3 &scaling)
{
	this->set_scaling(this->scaling*scaling);
}

void CoordinateTransform::translate_origin(const glm::vec3 &translation)
{
	this->set_origin_translation(this->origin_translation + translation);
}

glm::vec3 CoordinateTransform::transform_position(const glm::vec3 &position, const bool to_global)
{
	glm::vec4 transformed_position = (to_global ? this->get_global_matrix() : this->get_inverse_global_matrix())*glm::vec4(position, 1.0);
	transformed_position /= transformed_position.w;

	return glm::vec3(transformed_position);
}

glm::vec3 CoordinateTransform::transform_direction(const glm::vec3 &direction, const bool to_global)
{
	return glm::vec3((to_global ? this->get_global_matrix() : this->get_inverse_global_matrix())*glm::vec4(direction, 0.0));
}

void CoordinateTransform::set_matrices()
{
	if (this->is_updated)
		return;

	this->local_matrix = glm::translate(this->translation)*glm::mat4_cast(this->rotation)*glm::scale(this->scaling)*glm::translate(this->origin_translation);

	this->global_matrix = this->local_matrix;
	if (this->parent)
	{
		this->parent->set_matrices();
		this->global_matrix = this->parent->get_global_matrix()*this->global_matrix;
	}
	this->inverse_global_matrix = glm::inverse(this->global_matrix);

	this->set_updated(true);
}

void CoordinateTransform::set_updated(const bool is_updated)
{
	if (is_updated)
		this->is_updated = true;
	else
	{
		for (auto const &child : this->children)
			child.lock()->set_updated(false);
		this->is_updated = false;
	}
}
