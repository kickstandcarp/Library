#include "clock.hpp"



Clock::Clock(const float time, const float elapsed_time, const unsigned int step_count)
:   time(time),
    elapsed_time(elapsed_time),
    step_count(step_count)
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
