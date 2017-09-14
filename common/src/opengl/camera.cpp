#include <glm/gtc/matrix_transform.hpp>

#include "opengl/camera.hpp"



Camera::Camera(const float width, const float height, const float near_z, const float far_z, const bool orthographic)
:   width(width),
    height(height),
    near_z(near_z),
    far_z(far_z),
    is_orthographic(orthographic),
    is_updated(false),
    coordinate_transform(std::make_shared<CoordinateTransform>()),
	perspective_matrix(1.0f),
    transform_matrix(1.0f)
{
    this->set_perspective(width, height, near_z, far_z, orthographic);
}

Camera::~Camera()
{

}

const std::shared_ptr<CoordinateTransform> Camera::get_coordinate_transform()
{
    this->is_updated = false;
    return this->coordinate_transform;
}

float Camera::get_width() const
{
    return this->width;
}

float Camera::get_height() const
{
    return this->height;
}

float Camera::get_near_z() const
{
    return this->near_z;
}

float Camera::get_far_z() const
{
    return this->far_z;
}

bool Camera::get_orthographic() const
{
    return this->is_orthographic;
}

const glm::mat4x4& Camera::get_transform_matrix()
{
    if (!this->is_updated)
    {
        this->transform_matrix = this->perspective_matrix*this->coordinate_transform->get_inverse_global_matrix();
        this->is_updated = true;
    }

    return this->transform_matrix;
}

void Camera::set_perspective(const float width, const float height, const float near_z, const float far_z, const bool orthographic)
{
    this->is_updated = false;
    if (orthographic)
        this->perspective_matrix = glm::ortho(-0.5f*width, 0.5f*width, -0.5f*height, 0.5f*height, near_z, far_z);
    else
        this->perspective_matrix = glm::perspective(height, width / height, near_z, far_z);
}
