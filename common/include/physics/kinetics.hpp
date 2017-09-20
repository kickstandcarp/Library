#ifndef PHYSICS_KINETICS_HPP
#define PHYSICS_KINETICS_HPP

#include <memory>
#include <tuple>
#include "clock.hpp"
#include "geometry/segmented_path.hpp"

template <class ...A>
class Kinetics
{
	public:
		Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities, const std::tuple<A...> &external_accelerations);
		virtual ~Kinetics();

		template <unsigned int I> void						add_path(const float time);
		template <unsigned int I> void						remove_path();

        virtual void										step(const Clock &clock)=0;

        std::tuple<A...>									values, velocities, external_accelerations;
		
		std::tuple<std::shared_ptr<SegmentedPath<A> >...>	value_paths;
		float												path_duration;

    protected:
        template <unsigned int I> void						step_path(const Clock &clock);
};



template <class ...A>
Kinetics<A...>::Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities, const std::tuple<A...> &external_accelerations)
:   values(values),
    velocities(velocities),
	external_accelerations(external_accelerations),
	path_duration(0.0f)
{

}

template <class ...A>
Kinetics<A...>::~Kinetics()
{

}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::add_path(const float time)
{
	std::get<I>(this->value_paths) = std::make_shared<SegmentedPath<typename std::tuple_element<I, std::tuple<A...> >::type> >(std::list<PathVertex<typename std::tuple_element<I, std::tuple<A...> >::type> >(1, PathVertex<typename std::tuple_element<I, std::tuple<A...> >::type>(std::get<I>(values), time)));
}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::remove_path()
{
	std::get<I>(this->value_paths).reset();
}

template <class ...A>
template <unsigned int I>
void Kinetics<A...>::step_path(const Clock &clock)
{
	if (std::get<I>(this->value_paths))
	{
		std::get<I>(this->value_paths)->remove_path_vertices_prior(clock.time + clock.elapsed_time - this->path_duration);
        std::get<I>(this->value_paths)->add_path_vertex(PathVertex<typename std::tuple_element<I, std::tuple<A...> >::type>(std::get<I>(this->values), clock.time + clock.elapsed_time));
	}
}

#endif
