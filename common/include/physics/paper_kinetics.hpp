#ifndef PHYSICS_PAPER_KINETICS_HPP
#define PHYSICS_PAPER_KINETICS_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "clock.hpp"
#include "physics/kinetics.hpp"

class PaperKinetics: public Kinetics<glm::vec3, glm::quat>
{
	public:
		PaperKinetics(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity, const glm::vec3 &external_acceleration, const glm::vec3 &external_angular_acceleration, const float perpendicular_friction, const float parallel_friction, const float fluid_density, const float paper_density, const glm::vec3 &size);
		virtual ~PaperKinetics();

		const std::shared_ptr<SegmentedPath<glm::vec3> >	get_position_path();
		const std::shared_ptr<SegmentedPath<glm::quat> >	get_orientation_path();

		void												add_position_path(const float time);
		void												add_orientation_path(const float time);

		void												remove_position_path();
		void												remove_orientation_path();

        virtual void										step(const Clock &clock);

		float												perpendicular_friction, parallel_friction;
        float												fluid_density, paper_density;
		glm::vec3											size;
};

std::array<float, 6>										paper_kinetics_d2x_dt2(const float dt, const std::array<float, 6> &dx_dt, const PaperKinetics &kinetics);

#endif
