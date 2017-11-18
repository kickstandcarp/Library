#include <cmath>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "physics/paper_kinetics.hpp"
#include "physics/integration.hpp"



PaperKinetics::PaperKinetics(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity, const float perpendicular_friction, const float parallel_friction, const float fluid_density, const float paper_density, const glm::vec3 &size)
:   Kinetics(std::make_tuple(position, orientation), std::make_tuple(velocity, glm::quat(0.0f, angular_velocity))),
    perpendicular_friction(perpendicular_friction),
    parallel_friction(parallel_friction),
    fluid_density(fluid_density),
    paper_density(paper_density),
    size(size)
{
	this->external_accelerations = std::make_tuple(glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
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
	// this->Kinetics<glm::vec3, glm::quat>::runge_kutta_4(clock);
	this->Kinetics<glm::vec3, glm::quat>::adaptive_runge_kutta_45(clock);
	std::get<1>(this->values) = glm::normalize(std::get<1>(this->values));

	this->Kinetics<glm::vec3, glm::quat>::step_history<0>(clock);
    this->Kinetics<glm::vec3, glm::quat>::step_history<1>(clock);
}

std::tuple<glm::vec3, glm::quat> PaperKinetics::runge_kutta_midpoint(const float dt, const std::tuple<glm::vec3, glm::quat> &x, const std::vector<float> &coefficients, const std::vector<std::tuple<glm::vec3, glm::quat> > &dx_dts, const bool derivative) const
{
	glm::vec3 dx_dt(0.0f), dtheta_dt(0.0f);
	for (unsigned int i = 0; i < coefficients.size(); i++)
	{
		dx_dt += coefficients[i]*std::get<0>(dx_dts[i]);
		dtheta_dt += coefficients[i]*glm::vec3(std::get<1>(dx_dts[i]).x, std::get<1>(dx_dts[i]).y, std::get<1>(dx_dts[i]).z);
	}

	glm::vec3 x_midpoint = ingetrate(dt, std::get<0>(x), dx_dt);
	glm::quat theta_midpoint = derivative ? glm::quat(0.0f, ingetrate(dt, glm::vec3(std::get<1>(x).x, std::get<1>(x).y, std::get<1>(x).z), dtheta_dt)) : ingetrate(dt, std::get<1>(x), glm::quat(0.0f, dtheta_dt));

	return std::tuple<glm::vec3, glm::quat>(x_midpoint, theta_midpoint);
}

float PaperKinetics::adaptive_runge_kutta_relative_error(const std::tuple<glm::vec3, glm::quat> &dx_dt_1, const std::tuple<glm::vec3, glm::quat> &dx_dt_2) const
{
	float dx_dt_error = glm::distance(std::get<0>(dx_dt_1), std::get<0>(dx_dt_2));
	float dx_dt_magnitude = 0.5f*(glm::length(std::get<0>(dx_dt_1)) + glm::length(std::get<0>(dx_dt_2)));
	float dx_dt_relative_error = dx_dt_magnitude == 0.0f ? 0.0f : dx_dt_error / dx_dt_magnitude;

	float dtheta_dt_error = glm::distance(glm::vec3(std::get<1>(dx_dt_1).x, std::get<1>(dx_dt_1).y, std::get<1>(dx_dt_1).z), glm::vec3(std::get<1>(dx_dt_2).x, std::get<1>(dx_dt_2).y, std::get<1>(dx_dt_2).z));
	float dtheta_dt_magnitude = 0.5f*(glm::length(glm::vec3(std::get<1>(dx_dt_1).x, std::get<1>(dx_dt_1).y, std::get<1>(dx_dt_1).z)) + glm::length(glm::vec3(std::get<1>(dx_dt_2).x, std::get<1>(dx_dt_2).y, std::get<1>(dx_dt_2).z)));
	float dtheta_dt_relative_error = dtheta_dt_magnitude == 0.0f ? 0.0f : dtheta_dt_error / dtheta_dt_magnitude;

	return std::max(dx_dt_relative_error / this->relative_step_tolerance[0], dtheta_dt_relative_error / this->relative_step_tolerance[1]);
}

std::tuple<glm::vec3, glm::quat> PaperKinetics::d2x_dt2(const float t, const std::tuple<glm::vec3, glm::quat> &x, const std::tuple<glm::vec3, glm::quat> &dx_dt) const
{
	glm::vec3 friction_acceleration = glm::rotate(std::get<1>(x), -glm::vec3(this->parallel_friction, this->perpendicular_friction, this->parallel_friction)*glm::rotate(glm::inverse(std::get<1>(x)), std::get<0>(dx_dt)));
	glm::vec3 friction_angular_acceleration = -glm::vec3(this->perpendicular_friction, this->parallel_friction, this->perpendicular_friction)*glm::vec3(std::get<1>(dx_dt).x, std::get<1>(dx_dt).y, std::get<1>(dx_dt).z);

	float velocity_magnitude = glm::length(std::get<0>(dx_dt));
	glm::vec3 lift_acceleration(0.0f), lift_angular_acceleration(0.0f);
	if (velocity_magnitude != 0.0f)
	{
		glm::vec3 velocity_direction = std::get<0>(dx_dt) / velocity_magnitude;
		glm::vec3 normal = glm::rotate(std::get<1>(x), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::quat lift_rotation = glm::rotation(velocity_direction, normal);

		float angle_of_attack = glm::angle(lift_rotation);
		float lift_coefficient = glm::pi<float>()*(this->fluid_density / this->paper_density)*velocity_magnitude*velocity_magnitude*std::cos(angle_of_attack);

		lift_acceleration = -lift_coefficient*glm::rotate(glm::angleAxis(glm::half_pi<float>(), glm::axis(lift_rotation)), velocity_direction);
		lift_angular_acceleration = -3.0f*lift_coefficient*glm::rotate(glm::inverse(std::get<1>(x)), glm::cross(velocity_direction, normal)) / glm::vec3(this->size.y*this->size.y + this->size.z*this->size.z, this->size.x*this->size.x + this->size.z*this->size.z, this->size.x*this->size.x + this->size.y*this->size.y);
	}
	
	glm::vec3 acceleration = std::get<0>(this->external_accelerations) + friction_acceleration + lift_acceleration;
	glm::quat angular_acceleration = glm::quat(0.0f, glm::vec3(std::get<1>(this->external_accelerations).x, std::get<1>(this->external_accelerations).y, std::get<1>(this->external_accelerations).z) + friction_angular_acceleration + lift_angular_acceleration);

	return std::tuple<glm::vec3, glm::quat>(acceleration, angular_acceleration);
}
