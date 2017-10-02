#include "clock.hpp"



Clock::Clock()
:   time(0.0f),
    elapsed_time(0.0f),
    step_count(0)
{

}

Clock::~Clock()
{

}

void Clock::step(const float elapsed_time)
{
    this->time += this->elapsed_time;
    this->elapsed_time = elapsed_time;
    this->step_count++;
}
