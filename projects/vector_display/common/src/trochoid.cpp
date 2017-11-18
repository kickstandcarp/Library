#include <cmath>

#include "trochoid.hpp"



Trochoid::Trochoid(const float stator_radius, const float rotor_radius, const float rotor_offset, const float min_vertex_distance)
:   Curve(min_vertex_distance),
    stator_radius(stator_radius),
    rotor_radius(rotor_radius),
    rotor_offset(rotor_offset)
{

}

Trochoid::~Trochoid()
{

}

float Trochoid::get_min_t() const
{
    return 0.0f;
}

float Trochoid::get_max_t() const
{
    return INFINITY;
}

CurveVertex<glm::vec2> Trochoid::vertex(const float t) const
{
    float c = std::sqrt((this->stator_radius - this->rotor_radius)*(this->stator_radius - this->rotor_radius)*(this->rotor_offset*this->rotor_offset + this->rotor_radius*this->rotor_radius) / (this->rotor_radius*this->rotor_radius));
    float x = (this->stator_radius - this->rotor_radius)*std::cos(t / c) + this->rotor_offset*std::cos((this->stator_radius - this->rotor_radius)*(t / c) / this->rotor_radius);
    float y = (this->stator_radius - this->rotor_radius)*std::sin(t / c) - this->rotor_offset*std::sin((this->stator_radius - this->rotor_radius)*(t / c) / this->rotor_radius);
    return {glm::vec2(x, y), t};
}

std::list<CurveVertex<glm::vec2> > Trochoid::vertices(const float t1, const float t2) const
{
    std::list<CurveVertex<glm::vec2> > vertices{this->vertex(t1), this->vertex(t2)};
    this->fill_vertices(vertices);

    return vertices;
}
