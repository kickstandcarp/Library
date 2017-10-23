#include <cmath>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "physics/paper_kinetics.hpp"
#include "math/integration.hpp"
#include "math/arithmatic.hpp"
#include "parametric/segment_curve.hpp"



PaperKinetics::PaperKinetics(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity, const glm::vec3 &external_acceleration, const glm::vec3 &external_angular_acceleration, const float perpendicular_friction, const float parallel_friction, const float fluid_density, const float paper_density, const glm::vec3 &size)
:   Kinetics(std::make_tuple(position, orientation), std::make_tuple(velocity, glm::quat(0.0f, angular_velocity)), std::make_tuple(external_acceleration, glm::quat(0.0f, external_angular_acceleration))),
    perpendicular_friction(perpendicular_friction),
    parallel_friction(parallel_friction),
    fluid_density(fluid_density),
    paper_density(paper_density),
    size(size)
{

}

PaperKinetics::~PaperKinetics()
{

}

const std::shared_ptr<const SegmentCurve<glm::vec3> > PaperKinetics::get_position_history() const
{
	return std::get<0>(this->value_histories);
}

const std::shared_ptr<const SegmentCurve<glm::quat> > PaperKinetics::get_orientation_history() const
{
	return std::get<1>(this->value_histories);
}

void PaperKinetics::add_position_history(const float time)
{
	this->Kinetics<glm::vec3, glm::quat>::add_history<0>(time, CurveInterpolation::cubic);
}

void PaperKinetics::add_orientation_history(const float time)
{
	this->Kinetics<glm::vec3, glm::quat>::add_history<1>(time, CurveInterpolation::linear);
}

void PaperKinetics::remove_position_history()
{
	this->Kinetics<glm::vec3, glm::quat>::remove_history<0>();
}

void PaperKinetics::remove_orientation_history()
{
	this->Kinetics<glm::vec3, glm::quat>::remove_history<1>();
}

void PaperKinetics::step(const Clock &clock)
{
	std::array<float, 6> dx_dt{{std::get<0>(this->velocities).x, std::get<0>(this->velocities).y, std::get<0>(this->velocities).z, std::get<1>(this->velocities).x, std::get<1>(this->velocities).y, std::get<1>(this->velocities).z}};
	std::array<float, 6> d2x_dt2 = runge_kutta<std::array<float, 6>, const PaperKinetics&>(paper_kinetics_d2x_dt2, dx_dt, clock.time, clock.elapsed_time, *this);

	accumulate(std::get<0>(this->velocities), glm::vec3(d2x_dt2[0], d2x_dt2[1], d2x_dt2[2]), clock.elapsed_time);
	accumulate(std::get<0>(this->values), std::get<0>(this->velocities), clock.elapsed_time);

	accumulate(*reinterpret_cast<glm::vec4*>(&std::get<1>(this->velocities)), glm::vec4(d2x_dt2[3], d2x_dt2[4], d2x_dt2[5], 0.0f), clock.elapsed_time);
	accumulate(std::get<1>(this->values), std::get<1>(this->velocities), clock.elapsed_time);
	std::get<1>(this->values) = glm::normalize(std::get<1>(this->values));

	this->Kinetics<glm::vec3, glm::quat>::step_history<0>(clock);
	this->Kinetics<glm::vec3, glm::quat>::step_history<1>(clock);
}

std::array<float, 6> paper_kinetics_d2x_dt2(const float t, const std::array<float, 6> &dx_dt, const PaperKinetics &kinetics)
{
    glm::vec3 velocity(dx_dt[0], dx_dt[1], dx_dt[2]);
    glm::vec3 angular_velocity(dx_dt[3], dx_dt[4], dx_dt[5]);

	glm::quat orientation = std::get<1>(kinetics.values);

    glm::vec3 friction_acceleration = glm::rotate(orientation, -glm::vec3(kinetics.parallel_friction, kinetics.perpendicular_friction, kinetics.parallel_friction)*glm::rotate(glm::inverse(orientation), velocity));
    glm::vec3 friction_angular_acceleration = -glm::vec3(kinetics.perpendicular_friction, kinetics.parallel_friction, kinetics.perpendicular_friction)*angular_velocity;

    float velocity_magnitude = glm::length(velocity);
	glm::vec3 lift_acceleration(0.0f), lift_angular_acceleration(0.0f);
	if (velocity_magnitude != 0.0f)
    {
		glm::vec3 velocity_direction = velocity / velocity_magnitude;
		glm::vec3 normal = glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glm::quat lift_rotation = glm::rotation(velocity_direction, normal);

		float angle_of_attack = glm::angle(lift_rotation);
		float lift_coefficient = glm::pi<float>()*(kinetics.fluid_density / kinetics.paper_density)*velocity_magnitude*velocity_magnitude*std::cos(angle_of_attack);
		
		lift_acceleration = -lift_coefficient*glm::rotate(glm::angleAxis(glm::half_pi<float>(), glm::axis(lift_rotation)), velocity_direction);
		lift_angular_acceleration = -3.0f*lift_coefficient*glm::rotate(glm::inverse(orientation), glm::cross(velocity_direction, normal)) / glm::vec3(kinetics.size.y*kinetics.size.y + kinetics.size.z*kinetics.size.z, kinetics.size.x*kinetics.size.x + kinetics.size.z*kinetics.size.z, kinetics.size.x*kinetics.size.x + kinetics.size.y*kinetics.size.y);
	}

    glm::vec3 acceleration = friction_acceleration + lift_acceleration + std::get<0>(kinetics.external_accelerations);
    glm::vec3 angular_acceleration = friction_angular_acceleration + lift_angular_acceleration + glm::vec3(std::get<1>(kinetics.external_accelerations).x, std::get<1>(kinetics.external_accelerations).y, std::get<1>(kinetics.external_accelerations).z);

	return {{acceleration.x, acceleration.y, acceleration.z, angular_acceleration.x, angular_acceleration.y, angular_acceleration.z}};
}

float paper_kinetics_error(const std::array<float, 6> &x1, const std::array<float, 6> &x2)
{
	return distance(glm::vec3(x1[0], x1[1], x1[2]), glm::vec3(x2[0], x2[1], x2[2])) + distance(glm::vec3(x1[3], x1[4], x1[5]), glm::vec3(x2[3], x2[4], x2[5]));
}
