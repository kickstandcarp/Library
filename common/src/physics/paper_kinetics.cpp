#include <cmath>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "physics/paper_kinetics.hpp"
#include "math/integration.hpp"
#include "math/arithmatic.hpp"



PaperKinetics::PaperKinetics(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity, const float perpendicular_friction, const float parallel_friction, const float fluid_density, const float paper_density, const glm::vec3 &size, const glm::vec3 &acceleration)
:   perpendicular_friction(perpendicular_friction),
    parallel_friction(parallel_friction),
    fluid_density(fluid_density),
    paper_density(paper_density),
    size(size),
	acceleration(acceleration),
    position(position),
	orientation(orientation),
	velocity(velocity),
    angular_velocity(angular_velocity)
{

}

PaperKinetics::~PaperKinetics()
{

}

void PaperKinetics::step(const float elapsed_time)
{
    std::array<float, 6> dx_dt{{this->velocity.x, this->velocity.y, this->velocity.z, this->angular_velocity.x, this->angular_velocity.y, this->angular_velocity.z}};
    std::array<float, 6> d2x_dt2 = runge_kutta<std::array<float, 6>, const PaperKinetics&>(paper_kinetics_d2x_dt2, dx_dt, 0.0f, elapsed_time, *this);

    accumulate(this->velocity, glm::vec3(d2x_dt2[0], d2x_dt2[1], d2x_dt2[2]), elapsed_time);
    accumulate(this->position, this->velocity, elapsed_time);

    accumulate(this->angular_velocity, glm::vec3(d2x_dt2[3], d2x_dt2[4], d2x_dt2[5]), elapsed_time);
	accumulate(this->orientation, this->angular_velocity, elapsed_time);
}

std::array<float, 6> paper_kinetics_d2x_dt2(const float dt, const std::array<float, 6> &dx_dt, const PaperKinetics &kinetics)
{
    glm::vec3 velocity(dx_dt[0], dx_dt[1], dx_dt[2]);
    glm::vec3 angular_velocity(dx_dt[3], dx_dt[4], dx_dt[5]);

	glm::vec3 normal = glm::rotate(kinetics.orientation, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 friction_acceleration = glm::rotate(kinetics.orientation, -glm::vec3(kinetics.parallel_friction, kinetics.perpendicular_friction, kinetics.parallel_friction)*glm::rotate(glm::inverse(kinetics.orientation), velocity));
    glm::vec3 friction_angular_acceleration = -glm::vec3(kinetics.perpendicular_friction, kinetics.parallel_friction, kinetics.perpendicular_friction)*angular_velocity;

    float velocity_magnitude = glm::length(velocity);
    glm::vec3 lift_acceleration(0.0f), lift_angular_acceleration(0.0f);
	if (velocity_magnitude != 0.0f)
    {
		glm::vec3 velocity_direction = velocity / velocity_magnitude;
		glm::quat lift_rotation = glm::rotation(velocity_direction, normal);

		float angle_of_attack = glm::angle(lift_rotation);
		float lift_coefficient = glm::pi<float>()*(kinetics.fluid_density / kinetics.paper_density)*velocity_magnitude*velocity_magnitude*std::cos(angle_of_attack);
		lift_acceleration = -lift_coefficient*glm::rotate(glm::angleAxis(glm::half_pi<float>(), glm::axis(lift_rotation)), velocity_direction);
		lift_angular_acceleration = -3.0f*lift_coefficient*glm::cross(velocity_direction, normal) / glm::vec3(kinetics.size.y*kinetics.size.y + kinetics.size.z*kinetics.size.z, kinetics.size.x*kinetics.size.x + kinetics.size.z*kinetics.size.z, kinetics.size.x*kinetics.size.x + kinetics.size.y*kinetics.size.y);
	}

    glm::vec3 acceleration = friction_acceleration + lift_acceleration + kinetics.acceleration;
    glm::vec3 angular_acceleration = friction_angular_acceleration + lift_angular_acceleration;

    return {{acceleration.x, acceleration.y, acceleration.z, angular_acceleration.x, angular_acceleration.y, angular_acceleration.z}};
}
