#ifndef CLOCK_HPP
#define CLOCK_HPP

class Clock
{
	public:
		Clock(const float time=0.0f);
        ~Clock();

        void    update(const float elapsed_time);

		float           time, elapsed_time;
        unsigned int    count;
};

#endif
