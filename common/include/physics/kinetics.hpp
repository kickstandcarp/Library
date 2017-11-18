#ifndef PHYSICS_KINETICS_HPP
#define PHYSICS_KINETICS_HPP

#include <cmath>
#include <memory>
#include <tuple>
#include "clock.hpp"
#include "parametric/segment_curve.hpp"
#include "physics/integration.hpp"

template <class ...A>
class Kinetics
{
    public:
        Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities);
        virtual ~Kinetics();

        template <unsigned int I> void                      add_history(const float time, const CurveInterpolation interpolation);
        template <unsigned int I> void                      remove_history();

        virtual void                                        step(const Clock &clock)=0;

        std::tuple<A...>                                    values, velocities, external_accelerations;
        
        std::tuple<std::shared_ptr<SegmentCurve<A> >...>    value_histories;
        float                                               history_duration;

        float                                               min_step;
		std::array<float, sizeof...(A)>						relative_step_tolerance;

    protected:
        template <unsigned int I> void                      step_history(const Clock &clock);

		void												runge_kutta_4(const Clock &clock);
		void												adaptive_runge_kutta_45(const Clock &clock);

	private:
		virtual std::tuple<A...>							runge_kutta_midpoint(const float dt, const std::tuple<A...> &x, const std::vector<float> &coefficients, const std::vector<std::tuple<A...> > &dx_dts, const bool derivative) const=0;
		virtual float										adaptive_runge_kutta_relative_error(const std::tuple<A...> &dx_dt_1, const std::tuple<A...> &dx_dt_2) const=0;
		virtual std::tuple<A...>							d2x_dt2(const float t, const std::tuple<A...> &x, const std::tuple<A...> &dx_dt) const=0;
};



template <class ...A>
Kinetics<A...>::Kinetics(const std::tuple<A...> &values, const std::tuple<A...> &velocities)
:   values(values),
    velocities(velocities),
    history_duration(0.0f),
    min_step(INFINITY)
{
	for (unsigned int i = 0; i < sizeof...(A); i++)
		relative_step_tolerance[i] = INFINITY;
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
        std::get<I>(this->value_histories)->remove_segment_vertices_prior(clock.time + clock.elapsed_time - this->history_duration);
        std::get<I>(this->value_histories)->add_segment_vertex({std::get<I>(this->values), clock.time + clock.elapsed_time});
    }
}

template <class ...A>
void Kinetics<A...>::runge_kutta_4(const Clock &clock)
{
	float t;
	std::tuple<A...> x, dx_dt;
	
	std::vector<std::tuple<A...> > ks(4), ls(4);
	for (unsigned int i = 0; i < 4; i++)
	{
		t = clock.time + runge_kutta_4_weights[i]*clock.elapsed_time;
		x = this->runge_kutta_midpoint(clock.elapsed_time, this->values, runge_kutta_4_matrix[i], ks, false);
		dx_dt = this->runge_kutta_midpoint(clock.elapsed_time, this->velocities, runge_kutta_4_matrix[i], ls, true);

		ks[i] = dx_dt;
		ls[i] = this->d2x_dt2(t, x, dx_dt);
	}

	this->values = this->runge_kutta_midpoint(clock.elapsed_time, this->values, runge_kutta_4_nodes, ks, false);
	this->velocities = this->runge_kutta_midpoint(clock.elapsed_time, this->velocities, runge_kutta_4_nodes, ls, true);
}

template <class ...A>
void Kinetics<A...>::adaptive_runge_kutta_45(const Clock &clock)
{
	float elapsed_time = clock.elapsed_time, dt = clock.elapsed_time;
	while (elapsed_time > 0.0f)
	{
		float t;
		std::tuple<A...> x, dx_dt;

		std::vector<std::tuple<A...> > ks(6), ls(6);
		for (unsigned int i = 0; i < 6; i++)
		{
			t = clock.time + adaptive_runge_kutta_45_weights[i]*dt;
			x = this->runge_kutta_midpoint(dt, this->values, adaptive_runge_kutta_45_matrix[i], ks, false);
			dx_dt = this->runge_kutta_midpoint(dt, this->velocities, adaptive_runge_kutta_45_matrix[i], ls, true);

			ks[i] = dx_dt;
			ls[i] = this->d2x_dt2(t, x, dx_dt);
		}

		std::tuple<A...> x = this->runge_kutta_midpoint(dt, this->values, adaptive_runge_kutta_45_nodes[1], ks, false);
		std::tuple<A...> dx_dt = this->runge_kutta_midpoint(dt, this->velocities, adaptive_runge_kutta_45_nodes[1], ls, true);
        std::tuple<A...> dx_dt_ = this->runge_kutta_midpoint(dt, this->velocities, adaptive_runge_kutta_45_nodes[0], ls, true);

		float relative_error = this->adaptive_runge_kutta_relative_error(dx_dt, dx_dt_);
		if (relative_error <= 1.0f || dt <= this->min_step)
		{
			this->values = x_2;
			this->velocities = dx_dt_2;
			elapsed_time -= dt;
		}

		dt = std::min(elapsed_time, std::max(this->min_step, dt*0.9f*std::min(10.0f, std::max(0.1f, std::pow(relative_error, -0.2f)))));
	}
}

#endif
