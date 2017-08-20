#include <sstream>

#include "repr/repr.hpp"
#include "repr/physics_repr.hpp"



std::string	oscillator_kinetics_repr(const OscillatorKinetics &oscillator_kinetics)
{
	std::stringstream stream;
	stream << "OscillatorKinetics(";
	stream << "value=(" << to_python_string(oscillator_kinetics.value) << "), ";
	stream << "velocity=(" << to_python_string(oscillator_kinetics.velocity) << "))";
    return stream.str();
}

std::string	paper_kinetics_repr(const PaperKinetics &paper_kinetics)
{
	std::stringstream stream;
	stream << "PaperKinetics(";
	stream << "position=(" << to_python_string(paper_kinetics.position.x) << ", " << to_python_string(paper_kinetics.position.y) << ", " << to_python_string(paper_kinetics.position.z) << "), ";
	stream << "orientation=(" << to_python_string(paper_kinetics.orientation.x) << ", " << to_python_string(paper_kinetics.orientation.y) << ", " << to_python_string(paper_kinetics.orientation.z) << ", " << to_python_string(paper_kinetics.orientation.w) << "), ";
	stream << "velocity=(" << to_python_string(paper_kinetics.velocity.x) << ", " << to_python_string(paper_kinetics.velocity.y) << ", " << to_python_string(paper_kinetics.velocity.z) << "), ";
	stream << "angular_velocity=(" << to_python_string(paper_kinetics.angular_velocity.x) << ", " << to_python_string(paper_kinetics.angular_velocity.y) << ", " << to_python_string(paper_kinetics.angular_velocity.z) << "))";
	return stream.str();
}
