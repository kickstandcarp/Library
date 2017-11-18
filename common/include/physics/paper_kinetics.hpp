#ifndef PHYSICS_PAPER_KINETICS_HPP
#define PHYSICS_PAPER_KINETICS_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "clock.hpp"
#include "parametric/segment_curve.hpp"
#include "physics/kinetics.hpp"

class PaperKinetics: public Kinetics<glm::vec3, glm::quat>
{
    public:
        PaperKinetics(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &velocity, const glm::vec3 &angular_velocity, const float perpendicular_friction, const float parallel_friction, const float fluid_density, const float paper_density, const glm::vec3 &size);
        virtual ~PaperKinetics();

        const std::shared_ptr<const SegmentCurve<glm::vec3> >   get_position_history() const;
        const std::shared_ptr<const SegmentCurve<glm::quat> >   get_orientation_history() const;

        void                                                    add_position_history(const float time);
        void                                                    add_orientation_history(const float time);

        void                                                    remove_position_history();
        void                                                    remove_orientation_history();

        virtual void                                            step(const Clock &clock);

        float                                                   perpendicular_friction, parallel_friction;
        float                                                   fluid_density, paper_density;
        glm::vec3                                               size;

	private:
		virtual std::tuple<glm::vec3, glm::quat>				runge_kutta_midpoint(const float dt, const std::tuple<glm::vec3, glm::quat> &x, const std::vector<float> &coefficients, const std::vector<std::tuple<glm::vec3, glm::quat> > &dx_dts, const bool derivative) const;
		virtual float											adaptive_runge_kutta_relative_error(const std::tuple<glm::vec3, glm::quat> &dx_dt_1, const std::tuple<glm::vec3, glm::quat> &dx_dt_2) const;
		virtual std::tuple<glm::vec3, glm::quat>				d2x_dt2(const float t, const std::tuple<glm::vec3, glm::quat> &x, const std::tuple<glm::vec3, glm::quat> &dx_dt) const;
};

#endif
