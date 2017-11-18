#ifndef EXPLODE_MODE_PLAYER_HPP
#define EXPLODE_MODE_PLAYER_HPP

#include <memory>
#include <string>
#include "clock.hpp"
// #include "physics/acceleration_kinetics.hpp"
#include "physics/oscillator_kinetics.hpp"
#include "event/event_handler.hpp"
#include "opengl/window.hpp"
#include "opengl/camera.hpp"

class Player
{
    public:
        Player(const std::shared_ptr<CoordinateTransform> &move_coordinate_transform, const std::shared_ptr<CoordinateTransform> &aim_coordinate_transform, const float radius, const std::shared_ptr<Clock> &clock);
        virtual ~Player();

        glm::vec2                                           get_aim_angles() const;
        void                                                set_aim_angles(const glm::vec2& aim_angles);

        void                                                reset(const float aim_at_height);
		void                                                move();
		void                                                aim();

        void                                                playing();

        void												update(const EventHandler &event_handler);
		void												step(const Clock &clock);
		void												draw(Window* window);

        std::shared_ptr<CoordinateTransform>                move_coordinate_transform, aim_coordinate_transform;
        std::unique_ptr<AccelerationKinetics<float> >       move_horizontal_kinetics, move_vertical_kinetics;
        std::unique_ptr<AccelerationKinetics<glm::vec2> >   aim_kinetics;

		std::shared_ptr<CoordinateTransform>                reticle_coordinate_transform;

        std::unique_ptr<Line>                               reticle_line, reticle_point;

    private:
        float                                               move_radius;
        glm::vec2                                           aim_angles;
};

#endif
