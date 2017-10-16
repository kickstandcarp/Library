#ifndef CLOCK_HPP
#define CLOCK_HPP

class Clock
{
	public:
		Clock(const float time=0.0f, const float elapsed_time=0.0f, const unsigned int step_count=0);
        ~Clock();

        void            step(const float elapsed_time);

		float           time, elapsed_time;
        unsigned int    step_count;
};

#endif
