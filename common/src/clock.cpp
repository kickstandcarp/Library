#include "clock.hpp"



Clock::Clock(const float initial_time)
:   time(initial_time),
    elapsed_time(0.0f),
    count(0)
{

}

Clock::~Clock()
{

}

void Clock::update(const float elapsed_time)
{
    this->time += this->elapsed_time;
    this->elapsed_time = elapsed_time;
    this->count++;
}
