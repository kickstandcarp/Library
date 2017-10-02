#ifndef CLOCK_HPP
#define CLOCK_HPP

class Clock
{
	public:
		Clock();
        ~Clock();

        void            step(const float elapsed_time);

		float           time, elapsed_time;
        unsigned int    step_count;
};

#endif
