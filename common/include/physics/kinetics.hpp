#ifndef PHYSICS_KINETICS_HPP
#define PHYSICS_KINETICS_HPP

#include <cmath>
#include <memory>
#include <tuple>
#include "clock.hpp"
#include "parametric/segment_curve.hpp"

template <class ...A>
class Kinetics
{
	public:
		Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities, const std::tuple<A...> &external_accelerations);
		virtual ~Kinetics();

		template <unsigned int I> void						add_history(const float time, const CurveInterpolation interpolation);
		template <unsigned int I> void						remove_history();

        virtual void										step(const Clock &clock)=0;

        std::tuple<A...>									values, velocities, external_accelerations;
		
		std::tuple<std::shared_ptr<SegmentCurve<A> >...>	value_histories;
		float												history_duration;

		float												min_step, step_tolerance;

    protected:
        template <unsigned int I> void						step_history(const Clock &clock);
};



template <class ...A>
Kinetics<A...>::Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities, const std::tuple<A...> &external_accelerations)
:   values(values),
    velocities(velocities),
	external_accelerations(external_accelerations),
	history_duration(0.0f),
	min_step(INFINITY),
	step_tolerance(INFINITY)
{

}

template <class ...A>
Kinetics<A...>::~Kinetics()
{

}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::add_history(const float time, const CurveInterpolation interpolation)
{
	std::get<I>(this->value_histories) = std::make_shared<SegmentCurve<typename std::tuple_element<I, std::tuple<A...> >::type> >(std::list<CurveVertex<typename std::tuple_element<I, std::tuple<A...> >::type> >(1, {std::get<I>(values), time}), interpolation);
}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::remove_history()
{
	std::get<I>(this->value_histories).reset();
}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::step_history(const Clock &clock)
{
	if (std::get<I>(this->value_histories))
	{
		std::get<I>(this->value_histories)->remove_curve_vertices_prior(clock.time + clock.elapsed_time - this->history_duration);
        std::get<I>(this->value_histories)->add_curve_vertex({std::get<I>(this->values), clock.time + clock.elapsed_time});
	}
}

#endif
