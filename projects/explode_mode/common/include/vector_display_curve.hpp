#ifndef EXPLODE_MODE_VECTOR_DISPLAY_CURVE_HPP
#define EXPLODE_MODE_VECTOR_DISPLAY_CURVE_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "opengl/window.hpp"

class VectorDisplayCurve
{
    public:
        VectorDisplayCurve(const std::vector<glm::vec4> &vertices, Window &window, const std::string &vertex_array_name);
        ~VectorDisplayCurve();

		glm::vec4						get_first_vertex() const;
		glm::vec4						get_last_vertex() const;
		std::string						get_vertex_array_name() const;

        void							step(const Clock &clock, Window &window);

        void							draw(Window &window) const;

		float							start_time;
		float							velocity;

        glm::vec3						color;
        float							excitation_duration, excitation;
        float							excitation_time_constant, decay_time_constant;

        glm::vec2						translation;
        float							rotation;
        float							width, cap_height;

    private:
		std::vector<glm::vec4>			vertices;

		float							min_color_value;

		std::string						vertex_array_name;
		std::string						step_shader_name, draw_shader_name;
};

void									initialize_window_vector_display_curve(Window &window, const std::string &step_shader_name="vector_display_curve_step", const std::string &draw_shader_name="vector_display_curve_draw");

#endif
