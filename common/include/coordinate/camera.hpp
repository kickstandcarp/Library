#ifndef COORDINATE_CAMERA_HPP
#define COORDINATE_CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include "coordinate/coordinate_transform.hpp"

class Camera
{
    public:
		Camera(const float width, const float height, const float near_z, const float far_z, const bool orthographic);
        ~Camera();

        CoordinateTransform&                    get_coordinate_transform();
        float                                   get_width() const;
        float                                   get_height() const;
        float                                   get_near_z() const;
        float                                   get_far_z() const;
        bool                                    get_orthographic() const;

        const glm::mat4x4&                      get_transform_matrix();

        void                                    set_perspective(const float width, const float height, const float near_z, const float far_z, const bool orthographic);

    private:
        float                                   width, height, near_z, far_z;
        bool                                    is_orthographic;

        bool                                    is_updated;
        std::shared_ptr<CoordinateTransform>    coordinate_transform;
        glm::mat4x4                             perspective_matrix, transform_matrix;
};

#endif