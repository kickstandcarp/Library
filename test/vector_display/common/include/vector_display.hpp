#ifndef VECTOR_DISPLAY_HPP
#define VECTOR_DISPLAY_HPP

#include <string>
#include <map>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "clock.hpp"
#include "opengl/window.hpp"
#include "physics/oscillator_kinetics.hpp"
#include "image/gaussian_filter.hpp"
#include "vector_display_path.hpp"

class VectorDisplay
{
	public:
		VectorDisplay(Window &window, const std::array<unsigned int, 2> &size, const std::string &frame_buffer_name);
		~VectorDisplay();

		VectorDisplayPath&							get_path(const std::string &name);
        void										add_path(const std::string &name, VectorDisplayPath &path);
        void										remove_path(const std::string &name);

        void                                        step(const Clock &clock, Window &window);
        void                                        draw(Window &window);

		float								        threshold;

        float                                       excitation_time_constant;
		float										decay_time_constant_1, decay_time_constant_2;
		float										decay_time_constant_edge_1, decay_time_constant_edge_2;

        OscillatorKinetics<glm::vec2>				kinetics, beam_kinetics;

        GaussianFilter                              glow_filter;

	private:
		std::string									shader_name;
		std::string									vertex_array_name;
		std::string									frame_buffer_name;
		unsigned int								prev_value_color_attachment, next_value_color_attachment, intermediate_color_attachment;

		std::map<std::string, VectorDisplayPath>    paths;

		unsigned int								max_shader_vertices;
};

#endif
