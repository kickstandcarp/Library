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
#include "vector_display_beam.hpp"

class VectorDisplay
{
	public:
		VectorDisplay(Window &window, const std::array<unsigned int, 2> &size);
		~VectorDisplay();

	    std::vector<std::string>                    get_beam_names() const;
        VectorDisplayBeam&                          get_beam(const std::string &name);

        void										add_beam(const std::string &name, VectorDisplayBeam &curve, Window &window);

        void										remove_beam(const std::string &name, Window &window);

        void                                        step(const Clock &clock, Window &window, const std::vector<std::string> &curve_names=std::vector<std::string>());

        void                                        draw(Window &window);

		float								        threshold;

        OscillatorKinetics<glm::vec2>				kinetics, beam_kinetics;

        GaussianFilter                              glow_filter;

	private:
		std::array<unsigned int, 2>					size;

		std::string									beam_shader_name, accumulation_shader_name;
		std::string									vertex_array_name;
		std::string									frame_buffer_name;
		unsigned int								prev_value_color_attachment_index, next_value_color_attachment_index;
		unsigned int								accumulation_color_attachment_index, intermediate_color_attachment_index;

		std::map<std::string, VectorDisplayBeam>    beams;

		unsigned int								max_shader_vertices;
};

#endif
