#ifndef VECTOR_DISPLAY_VECTOR_DISPLAY_HPP
#define VECTOR_DISPLAY_VECTOR_DISPLAY_HPP

#include <string>
#include <map>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "opengl/window.hpp"
#include "physics/oscillator_kinetics.hpp"
#include "image/gaussian_filter.hpp"
#include "vector_display_curve.hpp"

class VectorDisplay
{
	public:
		VectorDisplay(Window &window, const std::array<unsigned int, 2> &size, const std::string &frame_buffer_name);
		~VectorDisplay();

		VectorDisplayCurve&							get_curve(const std::string &name);
        void										add_curve(const std::string &name, VectorDisplayCurve &curve, Window &window);
        void										remove_curve(const std::string &name, Window &window);

        void                                        step(const Clock &clock, Window &window);
        void                                        draw(Window &window);

		float								        threshold;

        float                                       excitation_time_constant;
		float										decay_time_constant_1, decay_time_constant_2;
		float										decay_time_constant_edge_1, decay_time_constant_edge_2;

        OscillatorKinetics<glm::vec2>				kinetics, beam_kinetics;

        GaussianFilter                              glow_filter;

	private:
		std::array<unsigned int, 2>					size;

		std::string									curve_shader_name, accumulation_shader_name;
		std::string									vertex_array_name;
		std::string									frame_buffer_name;
		unsigned int								prev_value_color_attachment_index, next_value_color_attachment_index;
		unsigned int								accumulation_color_attachment_index, intermediate_color_attachment_index;

		std::map<std::string, VectorDisplayCurve>   curves;

		unsigned int								max_shader_vertices;
};

#endif
