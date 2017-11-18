#ifndef OPENGL_CAMERA_HPP
#define OPENGL_CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include "geometry/coordinate_transform.hpp"

class Camera
{
    public:
        Camera(const float width, const float height, const float near_z, const float far_z, const bool orthographic);
        ~Camera();

        const std::shared_ptr<CoordinateTransform>			get_coordinate_transform();
		const std::shared_ptr<const CoordinateTransform>	get_coordinate_transform() const;
		float												get_width() const;
        float												get_height() const;
        float												get_near_z() const;
        float												get_far_z() const;
        bool												get_orthographic() const;

        const glm::mat4x4&									get_transform_matrix() const;

        void												set_perspective(const float width, const float height, const float near_z, const float far_z, const bool orthographic);

    private:
        float												width, height, near_z, far_z;
        bool												is_orthographic;

        bool												is_updated;
        std::shared_ptr<CoordinateTransform>				coordinate_transform;
        glm::mat4x4											perspective_matrix, transform_matrix;
};

#endif
