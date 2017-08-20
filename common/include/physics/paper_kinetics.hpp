#ifndef PHYSICS_PAPER_KINETICS_HPP
#define PHYSICS_PAPER_KINETICS_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class PaperKinetics
{
	public:
		PaperKinetics(const glm::vec3 &position=glm::vec3(0.0f), const glm::quat &orientation=glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3 &velocity=glm::vec3(0.0f), const glm::vec3 &angular_velocity=glm::vec3(0.0f), const float perpendicular_friction=10.0f, const float parallel_friction=0.1f, const float fluid_density=0.1f, const float paper_density=1.0f, const glm::vec3 &size=glm::vec3(1.0f, 0.0f, 1.0f), const glm::vec3 &acceleration=glm::vec3(0.0, -9.8f, 0.0f));
		~PaperKinetics();

        void            step(const float elapsed_time);

		float           perpendicular_friction, parallel_friction;
        float           fluid_density, paper_density;
		glm::vec3       size;
		glm::vec3       acceleration;

        glm::vec3       position;
        glm::quat       orientation; 
		glm::vec3		velocity, angular_velocity;
};

std::array<float, 6>    paper_kinetics_d2x_dt2(const float dt, const std::array<float, 6> &dx_dt, const PaperKinetics &kinetics);

#endif
