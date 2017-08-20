#ifndef REPR_PHYSICS_REPR_HPP
#define REPR_PHYSICS_REPR_HPP

#include <string>
#include <glm/glm.hpp>
#include "physics/oscillator_kinetics.hpp"
#include "physics/paper_kinetics.hpp"

std::string	oscillator_kinetics_repr(const OscillatorKinetics &oscillator_kinetics);
std::string	paper_kinetics_repr(const PaperKinetics &paper_kinetics);

#endif
